using Unity.Netcode;  // Unity Netcode for GameObjects ��� ���
using UnityEngine;

// �÷��̾��� ��ġ�� ȸ���� ��� ����ü
public struct SpawnInfo
{
    public Vector3 Position;     // ��ġ ����
    public Quaternion Rotation;  // ȸ�� ����

    // ������: ��ġ�� ȸ���� �޾Ƽ� �ʵ� �ʱ�ȭ
    public SpawnInfo(Vector3 position, Quaternion rotation)
    {
        Position = position;
        Rotation = rotation;
    }
}

// �÷��̾ ��Ʈ��ũ�� ���ӵ� �� �ڵ����� �����Ǵ� �Ŵ��� Ŭ����
public class PlayerSpawnManager : NetworkBehaviour
{
    public float randomPosition = 10f;  // ���� ��ġ �ֺ��� ������ ��ġ ������ �� ����

    // ��Ʈ��ũ���� ������Ʈ�� ������ �� ȣ��Ǵ� Unity Netcode �Լ�
    public override void OnNetworkSpawn()
    {
        if (IsServer)  // ���������� ���� ��ġ�� ����
        {
            var spawnPoints = InGameManager.Instance.spawnPoints;     // �̸� ������ ���� ������

            var newPoint = GetRandomSpawnPoint(spawnPoints);          // ���� ��ġ ������ ����
            transform.position = newPoint.Position;                   // �÷��̾� ��ġ ����
            transform.rotation = newPoint.Rotation;                   // �÷��̾� ȸ�� ����

            InGameManager.Instance.ConnectedUserNum++;                // ���� ���� �� ����
        }

        base.OnNetworkSpawn();  // �θ� Ŭ���� ���� ���� (�߿�)
    }

    // ���� ���� �� �ϳ��� �������� ��� ��ġ/ȸ�� ���� ��ȯ
    private SpawnInfo GetRandomSpawnPoint(Transform[] spawnPoints)
    {
        if (spawnPoints == null || spawnPoints.Length == 0)
        {
            return new SpawnInfo();                             // �⺻�� ��ȯ (0,0,0 ��ġ)
        }

        int index = Random.Range(0, spawnPoints.Length);        // ���� �ε��� ����
        Vector3 pos = spawnPoints[index].position;              // �⺻ ��ġ

        pos += Vector3.up * 1.5f;                               // ��¦ ��� ��ġ (�ٴڿ� ������ �ʵ���)
        pos += new Vector3(
            Random.Range(-randomPosition, randomPosition),      // X�� ������ ����
            0f,
            Random.Range(-randomPosition, randomPosition));     // Z�� ������ ����

        return new SpawnInfo(pos, spawnPoints[index].rotation); // ��ġ + ȸ�� ��ȯ
    }
}
