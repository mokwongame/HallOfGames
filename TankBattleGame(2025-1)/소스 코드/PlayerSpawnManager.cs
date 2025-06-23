using Unity.Netcode;  // Unity Netcode for GameObjects 기능 사용
using UnityEngine;

// 플레이어의 위치와 회전을 담는 구조체
public struct SpawnInfo
{
    public Vector3 Position;     // 위치 정보
    public Quaternion Rotation;  // 회전 정보

    // 생성자: 위치와 회전을 받아서 필드 초기화
    public SpawnInfo(Vector3 position, Quaternion rotation)
    {
        Position = position;
        Rotation = rotation;
    }
}

// 플레이어가 네트워크에 접속될 때 자동으로 스폰되는 매니저 클래스
public class PlayerSpawnManager : NetworkBehaviour
{
    public float randomPosition = 10f;  // 스폰 위치 주변에 무작위 위치 편차를 줄 범위

    // 네트워크에서 오브젝트가 스폰될 때 호출되는 Unity Netcode 함수
    public override void OnNetworkSpawn()
    {
        if (IsServer)  // 서버에서만 스폰 위치를 설정
        {
            var spawnPoints = InGameManager.Instance.spawnPoints;     // 미리 설정된 스폰 지점들

            var newPoint = GetRandomSpawnPoint(spawnPoints);          // 스폰 위치 무작위 선택
            transform.position = newPoint.Position;                   // 플레이어 위치 설정
            transform.rotation = newPoint.Rotation;                   // 플레이어 회전 설정

            InGameManager.Instance.ConnectedUserNum++;                // 접속 유저 수 증가
        }

        base.OnNetworkSpawn();  // 부모 클래스 로직 실행 (중요)
    }

    // 스폰 지점 중 하나를 무작위로 골라서 위치/회전 정보 반환
    private SpawnInfo GetRandomSpawnPoint(Transform[] spawnPoints)
    {
        if (spawnPoints == null || spawnPoints.Length == 0)
        {
            return new SpawnInfo();                             // 기본값 반환 (0,0,0 위치)
        }

        int index = Random.Range(0, spawnPoints.Length);        // 랜덤 인덱스 선택
        Vector3 pos = spawnPoints[index].position;              // 기본 위치

        pos += Vector3.up * 1.5f;                               // 살짝 띄운 위치 (바닥에 박히지 않도록)
        pos += new Vector3(
            Random.Range(-randomPosition, randomPosition),      // X축 무작위 편차
            0f,
            Random.Range(-randomPosition, randomPosition));     // Z축 무작위 편차

        return new SpawnInfo(pos, spawnPoints[index].rotation); // 위치 + 회전 반환
    }
}
