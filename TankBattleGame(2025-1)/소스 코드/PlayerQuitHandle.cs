using Unity.Netcode;
using UnityEngine;

// �÷��̾� ���� �� ��Ʈ��ũ�� �˸��� ������ �̱��� Ŭ����
public class PlayerQuitHandle : MonoBehaviour
{
    // �̱��� �ν��Ͻ� ������Ƽ
    public static PlayerQuitHandle Instance { get; private set; }

    // ������Ʈ ���� �� �̱��� ���� ����
    private void Awake()
    {
        // �̹� �ν��Ͻ��� �����ϰ� �ڽ��� �ƴϸ� �ߺ� ������Ʈ ����
        if (Instance && Instance != this)
        {
            Destroy(gameObject);
            return;
        }

        // �ν��Ͻ� �Ҵ� �� �� ��ȯ �ÿ��� �ı����� �ʵ��� ����
        Instance = this;
        DontDestroyOnLoad(gameObject);
    }

    // ���ø����̼��� ����� �� ȣ��Ǵ� �ݹ� �Լ�
    private void OnApplicationQuit()
    {
        // ��Ʈ��ũ �Ŵ����� ���ų� Ŭ���̾�Ʈ�� ����Ǿ� ���� ������ ����
        if (!NetworkManager.Singleton || !NetworkManager.Singleton.IsConnectedClient)
        {
            return;
        }

        // ���� �÷��̾� ������Ʈ�� ����
        var playerObject = NetworkManager.Singleton.SpawnManager.GetLocalPlayerObject();

        // �÷��̾� ������Ʈ�� �ְ� QuitNotifier ������Ʈ�� ������ ������
        if (playerObject && playerObject.TryGetComponent(out QuitNotifier notifier))
        {
            // ������ ���� �˸� RPC ȣ�� (������ �÷��̾� ���� �뺸)
            notifier.NotifyQuitServerRpc();
        }
    }
}
