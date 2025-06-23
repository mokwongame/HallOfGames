using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Netcode;
using Random = UnityEngine.Random;

// ���� ������ �����ϴ� �Ŵ��� Ŭ���� (��Ʈ��ũ �̱���)
public class InGameManager : NetworkBehaviour
{
    public Transform[] spawnPoints;          // �÷��̾� ������ ��ġ �迭

    public GameObject playerPrefab;          // �÷��̾� ������(������ ������Ʈ)

    public static InGameManager Instance { get; private set; }  // �̱��� �ν��Ͻ�

    public int ConnectedUserNum { get; set; } // ���� ���� ���� ��

    public int BulletDamage { get; private set; } // �Ѿ� ������

    public float playTime = 60;               // ���� �÷��� �ð� (��)

    public bool CanMove { get; set; }         // �÷��̾� �̵� ���� ����

    // Ŭ���̾�Ʈ ID���� (������, ����)�� �����ϴ� ��ųʸ�(ĳ��)
    private Dictionary<ulong, (string userId, int Score)> _playerCache = new();

    // �̱��� ���� ����: Awake���� �ν��Ͻ� �Ҵ� �� �ߺ� ����
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

    // ��Ʈ��ũ�� ������Ʈ�� ������ �� ȣ�� (��Ʈ��ũ �ʱ�ȭ)
    public override void OnNetworkSpawn()
    {
        if (IsServer)
        {
            // ������� �÷��̾� ���� ĳ�� �ʱ�ȭ
            if (_playerCache.Count > 0)
            {
                _playerCache.Clear();
            }
        }

        CanMove = false;       // �ʱ⿡�� �̵� �Ұ� ����
        BulletDamage = 20;     // �Ѿ� ������ ����

        // Ŭ���̾�Ʈ�� ���� ������ �� �ݹ� ���
        NetworkManager.OnClientDisconnectCallback += OnClientDisconnected;

        base.OnNetworkSpawn();
    }

    // ��Ʈ��ũ���� ������Ʈ�� ����� �� ȣ�� (���� ó��)
    public override void OnNetworkDespawn()
    {
        NetworkManager.OnClientDisconnectCallback -= OnClientDisconnected; // �ݹ� ����

        base.OnNetworkDespawn();
    }

    // Ŭ���̾�Ʈ ���� ���� �� ȣ��Ǵ� �ݹ� �Լ�
    private void OnClientDisconnected(ulong clientId)
    {
        ConnectedUserNum--;  // ���� ���� �� ����

        // ���� �����ڰ� 1�� ���϶�� ���� ���� ó��
        if (ConnectedUserNum <= 1)
        {
            NetworkManager.Singleton.Shutdown();            // ��Ʈ��ũ ����
            UiManagerTank.Instance.LoadEndScene();          // ���� ������ ��ȯ
        }
    }

    // ���� ���� Ÿ�̸� ���� (Ŭ���̾�Ʈ���� ����)
    [ClientRpc]
    private void StartGameCountDownClientRpc()
    {
        SoundManager.Instance.PlaySound(SoundType.ThemeTank, true);     // ����� ���
        CanMove = true;                                                 // �÷��̾� �̵� ���
        UiManagerTank.Instance.StartTimer();                            // UI Ÿ�̸� ����
    }

    // ������ ǥ�� Ÿ�̸� ���� (Ŭ���̾�Ʈ���� ����)
    [ClientRpc]
    private void StartScoreboardCountDownClientRpc()
    {
        UiManagerTank.Instance.StartScoreboardTimer(); // ������ UI Ÿ�̸� ����
    }

    // ���� ���� ��û �Լ� (���� ȣ�� �� Ŭ���̾�Ʈ ����)
    public void StartGame()
    {
        StartGameCountDownClientRpc();
    }

    // ������ Ÿ�̸� ���� ��û �Լ�
    public void StartScoreboardTimer()
    {
        StartScoreboardCountDownClientRpc();
    }

    // �������� ���� ���� ��û RPC (Ŭ���̾�Ʈ ������ �ƴ� ������ ȣ�� ����)
    [ServerRpc(RequireOwnership = false)]
    public void CollectScoresServerRpc()
    {
        if (!IsServer)
        {
            return;
        }

        // �÷��̾� ���� ����Ʈ ����
        var scoreList = new List<ScoreData>();
        foreach (var entry in _playerCache.Values)
        {
            scoreList.Add(new ScoreData
            {
                UserName = entry.userId,
                Score = entry.Score
            });
        }

        // ���� �������� ����
        scoreList.Sort((a, b) => b.Score.CompareTo(a.Score));

        // Ŭ���̾�Ʈ�鿡�� ������ ������Ʈ RPC ȣ��
        UpdateScoreBoardClientRpc(scoreList.ToArray());
    }

    // Ư�� �÷��̾� ������ ĳ�ÿ� �����ϰų� ����
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

    // Ŭ���̾�Ʈ���� ������ UI ������Ʈ �Լ� ȣ�� RPC
    [ClientRpc]
    private void UpdateScoreBoardClientRpc(ScoreData[] scoreData)
    {
        UiManagerTank.Instance.UpdateScoreUI(scoreData);
    }

    // �÷��̾� ������ ó�� �Լ� (�ڷ�ƾ ����)
    public void HandleRespawn(ulong clientId)
    {
        StartCoroutine(RespawnCoroutine(clientId));
    }

    // ������ �ڷ�ƾ: 2�� �� ������ ��ġ�� �÷��̾� ����
    private IEnumerator RespawnCoroutine(ulong clientId)
    {
        yield return new WaitForSeconds(2f);  // ���ð�

        // ������ ���� ��ġ ����
        var spawnPos = spawnPoints[Random.Range(0, spawnPoints.Length)].position;

        // �÷��̾� ������ �ν��Ͻ�ȭ �� ��Ʈ��ũ ����, �ش� Ŭ���̾�Ʈ ������ ����
        var player = Instantiate(playerPrefab, spawnPos, Quaternion.identity);
        player.GetComponent<NetworkObject>().SpawnAsPlayerObject(clientId, true);
    }

    // �������� ��� �÷��̾� ������Ʈ ���� ��û �Լ� (�ܺ� ȣ���)
    public void RequestDestroyAllClients()
    {
        if (!IsServer)
        {
            return;
        }

        RequestDestroyAllClientsServerRpc();
    }

    // ���� RPC: �������� ��� �÷��̾� �±��� ��Ʈ��ũ ������Ʈ�� ã�� Despawn ó��
    [ServerRpc(RequireOwnership = false)]
    private void RequestDestroyAllClientsServerRpc()
    {
        if (!IsServer)
        {
            return;
        }

        // ���� ���� �����ϴ� ��� NetworkObject ã��
        var networkObjects = FindObjectsByType<NetworkObject>(FindObjectsSortMode.InstanceID);

        // �÷��̾� �±��̸� ������ ������Ʈ�� ����
        foreach (var obj in networkObjects)
        {
            if (obj.CompareTag("Player") && obj.IsSpawned)
            {
                obj.Despawn();
            }
        }
    }
}
//  �������� �ڵ�� ������ ������ ���� ������� �����Դϴ�.. 