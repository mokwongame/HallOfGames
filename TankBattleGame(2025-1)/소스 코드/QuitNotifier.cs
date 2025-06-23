using Unity.Netcode;

// Ŭ���̾�Ʈ�� ���Ḧ ��û���� �� �������� ó���ϴ� Ŭ����
public class QuitNotifier : NetworkBehaviour
{
    // Ŭ���̾�Ʈ�� ���Ḧ ������ �˸��� ServerRpc �޼���
    // RequireOwnership = false �� ȣ���ڰ� ������Ʈ�� �����ڰ� �ƴϾ ȣ�� ����
    [ServerRpc(RequireOwnership = false)]
    public void NotifyQuitServerRpc(ServerRpcParams rpcParams = default)
    {
        // ��û�� ���� Ŭ���̾�Ʈ�� ID�� ������
        ulong clientId = rpcParams.Receive.SenderClientId;

        // �ش� Ŭ���̾�Ʈ�� ���� ����Ǿ� �ִ��� Ȯ��
        if (NetworkManager.Singleton.ConnectedClients.TryGetValue(clientId, out var client))
        {
            var playerObj = client.PlayerObject;

            // �÷��̾� ������Ʈ�� �����ϸ� ��Ʈ��ũ���� ���� (Despawn)
            if (playerObj)
            {
                playerObj.Despawn();  // ��Ʈ��ũ���� �÷��̾� ����
            }
        }
    }
}