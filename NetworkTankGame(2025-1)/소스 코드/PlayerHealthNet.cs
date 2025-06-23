using UnityEngine;
using Unity.Netcode;
using Unity.Collections;

public class PlayerHealthNet : NetworkBehaviour
{
    public NetworkVariable<int> health = new NetworkVariable<int>(default, NetworkVariableReadPermission.Everyone, NetworkVariableWritePermission.Owner);
    public NetworkVariable<FixedString128Bytes> userId = new NetworkVariable<FixedString128Bytes>("player", NetworkVariableReadPermission.Everyone, NetworkVariableWritePermission.Owner);

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    /*
     [��Ʈ��ũ�󿡼� ���� �浹 ������ �߻���]
     - Host�� ��ź�� ������ �ſ� ���ϰ� �и�
     - Client�� ��ź�� �¾Ƶ� ���� �и��� �ʰų� �ƿ� ������ ���� ������ �߻���
     - Unity Netcode for GameObjects�� Rigidbody ���� ����(AddForce ��)�� 
       �ش� ������Ʈ�� Owner�� ���� ������ �� �ֵ��� ����
     
     - BombNet���� Server ���忡�� ���� �÷��̾� Rigidbody�� 
       ���� AddForce�� ȣ���ϸ�, �̴� Host�� Rigidbody���� ��������� 
       Client�� Rigidbody���� ���õ� (���ʰ� �ƴ�)
          
     - �浹�� �÷��̾�� ���� ���͸� �����ϰ�
       �� �÷��̾ �ڱ� Rigidbody���� Force�� �����ϵ��� RPC ȣ�� ������ ������
     */

    [Rpc(SendTo.Owner)]
    public void f_ApplyPushForceRpc(Vector3 vDirection) //���ۿ� ���� �����ϴ� RPC �޼ҵ�
    {
        Rigidbody rb = GetComponent<Rigidbody>();
        if (rb != null)
        {
            float fPushPower = 500.0f; //�и� ����
            rb.AddForce(vDirection * fPushPower, ForceMode.Impulse); //Impulse ���� ���� ����
        }
    }

    [Rpc(SendTo.Owner)]
    public void decHealthByAmountRpc(int nAmount)
    {
        int curValue = health.Value - nAmount;
        if (curValue < 0) curValue = 0;
        health.Value = curValue;
        Debug.Log($"health -= {nAmount} �� {health.Value}");
    }

    public override void OnNetworkSpawn()
    {
        if (IsOwner)
        {
            health.Value = UiManagerTank.Instance.maxHealth; // write
            health.OnValueChanged += OnHealthChanged; // �̺�Ʈ �ڵ鷯 ���
            userId.Value = GameManager.Instance.UserId;
            Debug.Log($"health = {health.Value}");
        }
        base.OnNetworkSpawn();
    }

    void OnHealthChanged(int prevValue, int newValue)
    {
        // newValue�� health.Value�� ��
        UiManagerTank.Instance.health = newValue;
        UiManagerTank.Instance.updateTextUserInfo();
        UiManagerTank.Instance.updateSliderHealth();
        //Debug.Log($"health = {UiManagerTank.Instance.health}");
    }

    public override void OnNetworkDespawn()
    {
        if (IsOwner)
        {
            health.OnValueChanged -= OnHealthChanged; // �̺�Ʈ �ڵ鷯 ����
        }
        base.OnNetworkDespawn();
    }

    public void decHealth()
    {
        int curValue = health.Value - 1;
        if (curValue < 0) curValue = 0;
        health.Value = curValue;
        Debug.Log($"health = {health.Value}");
    }

    [Rpc(SendTo.Owner)]
    public void decHealthRpc() // Ȯ���ϰ� RPC�� NetworkVariable ����
    {
        decHealth();
    }

    private void OnGUI() // GUI�� ó���ϴ� �̺�Ʈ ó����
    {
        Vector3 offset = new Vector3(-3.0f, 4.0f, 0.0f);
        Vector3 pos = Camera.main.WorldToScreenPoint(transform.position + offset);
        Rect rect = new Rect(0, 0, 100, 50);
        rect.x = pos.x;
        rect.y = Screen.height - pos.y;
        // GUI style ���
        GUIStyle style = new GUIStyle();
        style.richText = true; // HTML �±� ����� ���
        string text = $"<color=blue>{userId.Value}</color>: <color=red>{health.Value}</color>";
        GUI.Label(rect, text);
    }
}
