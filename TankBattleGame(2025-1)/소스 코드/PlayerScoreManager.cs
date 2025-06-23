using Unity.Collections;
using Unity.Netcode;

// �÷��̾� ������ ����� ID�� �����ϴ� Ŭ����
public class PlayerScoreManager : NetworkBehaviour
{
    // ��Ʈ��ũ ������ ����� ID�� ���� (�ִ� 64����Ʈ ���� ���� ���ڿ�)
    public NetworkVariable<FixedString64Bytes> userId = new();

    // ��Ʈ��ũ�� ������Ʈ�� ������ �� ȣ��
    public override void OnNetworkSpawn()
    {
        // ���� �������� ��� ������ ����� ID�� �����ϵ��� RPC ȣ��
        if (IsOwner)
        {
            SetUserIdServerRpc(BeginGameManager.Instance.UserId);
        }

        base.OnNetworkSpawn();
    }

    // ���� RPC: Ư�� Ŭ���̾�Ʈ �������� ������ ������ �߰� ��û
    // RequireOwnership = false �̹Ƿ� �����ڰ� �ƴϾ ȣ�� ����
    [ServerRpc(RequireOwnership = false)]
    public void AddScoreServerRpc(ulong ownerClientId, int amount)
    {
        // ������ InGameManager�� ���� ���� �Լ� ȣ��
        InGameManager.Instance.CachePlayerScore(ownerClientId, userId.Value.Value, amount);
    }

    // ���� RPC: ����� �̸��� ������ �����ϴ� �Լ� (�����)
    [ServerRpc(RequireOwnership = false)]
    private void SetUserIdServerRpc(string userName)
    {
        // ��Ʈ��ũ ������ ����� ID ����
        userId.Value = userName;
    }
}
