using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Netcode;
using Random = UnityEngine.Random;

// 게임 진행을 관리하는 매니저 클래스 (네트워크 싱글턴)
public class InGameManager : NetworkBehaviour
{
    public Transform[] spawnPoints;          // 플레이어 리스폰 위치 배열

    public GameObject playerPrefab;          // 플레이어 프리팹(생성할 오브젝트)

    public static InGameManager Instance { get; private set; }  // 싱글턴 인스턴스

    public int ConnectedUserNum { get; set; } // 현재 접속 유저 수

    public int BulletDamage { get; private set; } // 총알 데미지

    public float playTime = 60;               // 게임 플레이 시간 (초)

    public bool CanMove { get; set; }         // 플레이어 이동 가능 여부

    // 클라이언트 ID별로 (유저명, 점수)를 저장하는 딕셔너리(캐시)
    private Dictionary<ulong, (string userId, int Score)> _playerCache = new();

    // 싱글턴 패턴 구현: Awake에서 인스턴스 할당 및 중복 방지
    private void Awake()
    {
        if (!Instance)
        {
            Instance = this;
        }
        else if (Instance != this)
        {
            Destroy(gameObject);
        }
    }

    // 네트워크에 오브젝트가 스폰될 때 호출 (네트워크 초기화)
    public override void OnNetworkSpawn()
    {
        if (IsServer)
        {
            // 서버라면 플레이어 점수 캐시 초기화
            if (_playerCache.Count > 0)
            {
                _playerCache.Clear();
            }
        }

        CanMove = false;       // 초기에는 이동 불가 상태
        BulletDamage = 20;     // 총알 데미지 설정

        // 클라이언트가 접속 종료할 때 콜백 등록
        NetworkManager.OnClientDisconnectCallback += OnClientDisconnected;

        base.OnNetworkSpawn();
    }

    // 네트워크에서 오브젝트가 사라질 때 호출 (종료 처리)
    public override void OnNetworkDespawn()
    {
        NetworkManager.OnClientDisconnectCallback -= OnClientDisconnected; // 콜백 해제

        base.OnNetworkDespawn();
    }

    // 클라이언트 접속 종료 시 호출되는 콜백 함수
    private void OnClientDisconnected(ulong clientId)
    {
        ConnectedUserNum--;  // 접속 유저 수 감소

        // 남은 접속자가 1명 이하라면 게임 종료 처리
        if (ConnectedUserNum <= 1)
        {
            NetworkManager.Singleton.Shutdown();            // 네트워크 종료
            UiManagerTank.Instance.LoadEndScene();          // 종료 씬으로 전환
        }
    }

    // 게임 시작 타이머 시작 (클라이언트에서 실행)
    [ClientRpc]
    private void StartGameCountDownClientRpc()
    {
        SoundManager.Instance.PlaySound(SoundType.ThemeTank, true);     // 배경음 재생
        CanMove = true;                                                 // 플레이어 이동 허용
        UiManagerTank.Instance.StartTimer();                            // UI 타이머 시작
    }

    // 점수판 표시 타이머 시작 (클라이언트에서 실행)
    [ClientRpc]
    private void StartScoreboardCountDownClientRpc()
    {
        UiManagerTank.Instance.StartScoreboardTimer(); // 점수판 UI 타이머 시작
    }

    // 게임 시작 요청 함수 (서버 호출 → 클라이언트 실행)
    public void StartGame()
    {
        StartGameCountDownClientRpc();
    }

    // 점수판 타이머 시작 요청 함수
    public void StartScoreboardTimer()
    {
        StartScoreboardCountDownClientRpc();
    }

    // 서버에서 점수 수집 요청 RPC (클라이언트 소유자 아닌 누구나 호출 가능)
    [ServerRpc(RequireOwnership = false)]
    public void CollectScoresServerRpc()
    {
        if (!IsServer)
        {
            return;
        }

        // 플레이어 점수 리스트 생성
        var scoreList = new List<ScoreData>();
        foreach (var entry in _playerCache.Values)
        {
            scoreList.Add(new ScoreData
            {
                UserName = entry.userId,
                Score = entry.Score
            });
        }

        // 점수 내림차순 정렬
        scoreList.Sort((a, b) => b.Score.CompareTo(a.Score));

        // 클라이언트들에게 점수판 업데이트 RPC 호출
        UpdateScoreBoardClientRpc(scoreList.ToArray());
    }

    // 특정 플레이어 점수를 캐시에 저장하거나 갱신
    public void CachePlayerScore(ulong clientId, string userName, int score)
    {
        if (_playerCache.TryGetValue(clientId, out var oldData))
        {
            _playerCache[clientId] = (userName, oldData.Score + score);
        }
        else
        {
            _playerCache[clientId] = (userName, score);
        }
    }

    // 클라이언트에서 점수판 UI 업데이트 함수 호출 RPC
    [ClientRpc]
    private void UpdateScoreBoardClientRpc(ScoreData[] scoreData)
    {
        UiManagerTank.Instance.UpdateScoreUI(scoreData);
    }

    // 플레이어 리스폰 처리 함수 (코루틴 실행)
    public void HandleRespawn(ulong clientId)
    {
        StartCoroutine(RespawnCoroutine(clientId));
    }

    // 리스폰 코루틴: 2초 후 무작위 위치에 플레이어 생성
    private IEnumerator RespawnCoroutine(ulong clientId)
    {
        yield return new WaitForSeconds(2f);  // 대기시간

        // 무작위 스폰 위치 선택
        var spawnPos = spawnPoints[Random.Range(0, spawnPoints.Length)].position;

        // 플레이어 프리팹 인스턴스화 및 네트워크 스폰, 해당 클라이언트 소유로 설정
        var player = Instantiate(playerPrefab, spawnPos, Quaternion.identity);
        player.GetComponent<NetworkObject>().SpawnAsPlayerObject(clientId, true);
    }

    // 서버에서 모든 플레이어 오브젝트 제거 요청 함수 (외부 호출용)
    public void RequestDestroyAllClients()
    {
        if (!IsServer)
        {
            return;
        }

        RequestDestroyAllClientsServerRpc();
    }

    // 서버 RPC: 서버에서 모든 플레이어 태그의 네트워크 오브젝트를 찾아 Despawn 처리
    [ServerRpc(RequireOwnership = false)]
    private void RequestDestroyAllClientsServerRpc()
    {
        if (!IsServer)
        {
            return;
        }

        // 현재 씬에 존재하는 모든 NetworkObject 찾기
        var networkObjects = FindObjectsByType<NetworkObject>(FindObjectsSortMode.InstanceID);

        // 플레이어 태그이며 스폰된 오브젝트만 제거
        foreach (var obj in networkObjects)
        {
            if (obj.CompareTag("Player") && obj.IsSpawned)
            {
                obj.Despawn();
            }
        }
    }
}
//  누군가의 코드와 유사한 이유는 같이 만들었기 때문입니다.. 