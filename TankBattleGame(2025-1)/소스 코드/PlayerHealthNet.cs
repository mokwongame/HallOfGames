using Unity.Collections;
using Unity.Netcode;
using UnityEngine;

public class PlayerHealthNet : NetworkBehaviour
{
    private const int MaxHealth = 100;  // �ִ� ü��

    private const int GUIWidth = 100;   // GUI ǥ�� ���� ũ��
    private const int GUIHeight = 50;   // GUI ǥ�� ���� ũ��

    // ��Ʈ��ũ ������ ü�� ����, ��ΰ� ���� �� �ְ� �����ڸ� �� �� ����
    public NetworkVariable<int> health = new(0, NetworkVariableReadPermission.Everyone, NetworkVariableWritePermission.Owner);

    // ��Ʈ��ũ ������ ����� ID ����, ��ΰ� ���� �� �ְ� �����ڸ� �� �� ����
    public NetworkVariable<FixedString64Bytes> userId = new(default, NetworkVariableReadPermission.Everyone, NetworkVariableWritePermission.Owner);

    private Camera _mainCamera;         // ���� ī�޶� ����

    private void Start()
    {
        // ���� ī�޶� �Ҵ�
        _mainCamera = Camera.main;
    }

    // ��Ʈ��ũ�� ������Ʈ�� ������ �� ȣ��
    public override void OnNetworkSpawn()
    {
        // ������ Ŭ���̾�Ʈ��� �ʱ� ü�°� ����� ID ����, UI ����
        if (IsOwner)
        {
            health.Value = MaxHealth;
            userId.Value = BeginGameManager.Instance.UserId;
            UiManagerTank.Instance.UpdateUIInfo(health.Value);
        }

        base.OnNetworkSpawn();
    }

    // ü�� ���� ó�� �Լ� (���� �����ڿ��� ȣ��)
    private void DecHealth(int damage)
    {
        // ���� ü�¿��� �������� ��
        int curValue = health.Value - damage;

        // UI ���� (ü�� ǥ��)
        UiManagerTank.Instance.UpdateUIInfo(curValue);

        // ��Ʈ��ũ ���� ü�� ����
        health.Value = curValue;

        // ü���� 0 ���ϰ� �Ǹ� ������ ������ ��û �� ������Ʈ �ı� ȣ��
        if (curValue <= 0)
        {
            RequestRespawnServerRpc(OwnerClientId);
            DestroyEventRpc();
        }
    }

    // ���� RPC: ������ ��û, Ŭ���̾�Ʈ ID�� ������ ����
    [ServerRpc(RequireOwnership = false)]
    private void RequestRespawnServerRpc(ulong clientId)
    {
        // ������ InGameManager�� ������ ó�� ����
        InGameManager.Instance.HandleRespawn(clientId);
    }

    // ������ ������ RPC: ��Ʈ��ũ ������Ʈ ����(�ı�) ��û
    [Rpc(SendTo.Server)]
    private void DestroyEventRpc()
    {
        NetworkObject.Despawn();
    }

    // ������ Ŭ���̾�Ʈ�� ������ RPC: ü�� ���� ó�� �Լ� ȣ��
    [Rpc(SendTo.Owner)]
    public void DecHealthRpc(int damage)
    {
        DecHealth(damage);
    }

    // ����Ƽ OnGUI �Լ�: ȭ�鿡 �÷��̾� ü�°� ����� ID�� ǥ��
    private void OnGUI()
    {
        if (!_mainCamera)
        {
            return;
        }

        // ĳ���� �Ӹ� ���� 2 ������ŭ �ø� ��ġ
        Vector3 offset = new Vector3(0f, 2f, 0f);

        // ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ
        Vector3 pos = _mainCamera.WorldToScreenPoint(transform.position + offset);

        // ȭ�� ��ǥ �������� ǥ���� ���� ����
        Rect rect = new Rect(0, 0, GUIWidth, GUIHeight)
        {
            x = pos.x,
            // GUI�� ���� �ϴ��� (0,0)�̹Ƿ� y�� ���� �ʿ�
            y = Screen.height - pos.y
        };

        // ǥ���� �ؽ�Ʈ: ����� ID�� ü�� ��
        string text = $"{userId.Value} : {health.Value}";

        // GUI ���̺�� ȭ�鿡 ǥ��
        GUI.Label(rect, text);
    }
}
