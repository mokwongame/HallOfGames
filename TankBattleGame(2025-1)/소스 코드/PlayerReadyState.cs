using Unity.Netcode;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

// �÷��̾���� �غ� ���¸� �����ϴ� Ŭ����
public class PlayerReadyState : NetworkBehaviour
{
    // Ŭ���̾�Ʈ ID(ulong)�� Ű��, �غ� ����(bool)�� ������ �����ϴ� ��ųʸ�
    private Dictionary<ulong, bool> _readyStates = new();

    // �̱��� �ν��Ͻ�
    public static PlayerReadyState Instance;

    // ������Ʈ ���� �� ȣ��Ǵ� �ʱ�ȭ �Լ�
    private void Awake()
    {
        // �̱��� ���� ����: �ν��Ͻ��� ������ �ڽ��� �Ҵ�
        if (!Instance)
        {
            Instance = this;
        }

        // ���� �غ� ���� ��� �ʱ�ȭ
        _readyStates.Clear();
    }

    // �������� ����Ǵ� RPC: Ŭ���̾�Ʈ�� �غ� ���� ���� �� ȣ���
    // RequireOwnership = false �̹Ƿ� �����ڰ� �ƴϾ ȣ�� ����
    [ServerRpc(RequireOwnership = false)]
    public void SubmitReadyServerRpc(ServerRpcParams rpcParams = default)
    {
        // �غ� ���¸� ���� Ŭ���̾�Ʈ ID�� �޾ƿ�
        ulong clientId = rpcParams.Receive.SenderClientId;

        // �ش� Ŭ���̾�Ʈ�� �غ� ���¸� true�� ����
        _readyStates[clientId] = true;

        // ��� �÷��̾ �غ�Ǿ����� üũ�ϰ�, �غ�Ǿ����� ���� ����
        if (AllPlayersReady())
        {
            InGameManager.Instance.StartGame();
        }
    }

    // ��� �÷��̾ �غ� �������� Ȯ���ϴ� �Լ�
    private bool AllPlayersReady()
    {
        // �غ� ���� ��ųʸ��� ����� �÷��̾� ����
        // ���� ��Ʈ��ũ�� ����� Ŭ���̾�Ʈ ������ ������ false
        if (_readyStates.Count < NetworkManager.Singleton.ConnectedClients.Count)
        {
            return false;
        }

        // �÷��̾� ���� 1�� �����̸� false (2�� �̻���� ���� ���� ����)
        if (InGameManager.Instance.ConnectedUserNum <= 1)
        {
            return false;
        }

        // ��� �غ� ���°� true���� true ��ȯ (��� �غ��)
        return _readyStates.Values.All(isReady => isReady);
    }
}
