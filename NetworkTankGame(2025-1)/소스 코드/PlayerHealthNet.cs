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
     [네트워크상에서 물리 충돌 문제가 발생함]
     - Host는 포탄에 맞으면 매우 강하게 밀림
     - Client는 포탄에 맞아도 거의 밀리지 않거나 아예 반응이 없는 문제가 발생함
     - Unity Netcode for GameObjects는 Rigidbody 물리 연산(AddForce 등)을 
       해당 오브젝트의 Owner만 직접 수행할 수 있도록 제한
     
     - BombNet에서 Server 입장에서 상대방 플레이어 Rigidbody에 
       직접 AddForce를 호출하면, 이는 Host의 Rigidbody에는 적용되지만 
       Client의 Rigidbody에는 무시됨 (오너가 아님)
          
     - 충돌한 플레이어에게 방향 벡터를 전달하고
       각 플레이어가 자기 Rigidbody에만 Force를 적용하도록 RPC 호출 구조를 변경함
     */

    [Rpc(SendTo.Owner)]
    public void f_ApplyPushForceRpc(Vector3 vDirection) //반작용 힘을 적용하는 RPC 메소드
    {
        Rigidbody rb = GetComponent<Rigidbody>();
        if (rb != null)
        {
            float fPushPower = 500.0f; //밀림 강도
            rb.AddForce(vDirection * fPushPower, ForceMode.Impulse); //Impulse 모드로 힘을 가함
        }
    }

    [Rpc(SendTo.Owner)]
    public void decHealthByAmountRpc(int nAmount)
    {
        int curValue = health.Value - nAmount;
        if (curValue < 0) curValue = 0;
        health.Value = curValue;
        Debug.Log($"health -= {nAmount} → {health.Value}");
    }

    public override void OnNetworkSpawn()
    {
        if (IsOwner)
        {
            health.Value = UiManagerTank.Instance.maxHealth; // write
            health.OnValueChanged += OnHealthChanged; // 이벤트 핸들러 등록
            userId.Value = GameManager.Instance.UserId;
            Debug.Log($"health = {health.Value}");
        }
        base.OnNetworkSpawn();
    }

    void OnHealthChanged(int prevValue, int newValue)
    {
        // newValue가 health.Value가 됨
        UiManagerTank.Instance.health = newValue;
        UiManagerTank.Instance.updateTextUserInfo();
        UiManagerTank.Instance.updateSliderHealth();
        //Debug.Log($"health = {UiManagerTank.Instance.health}");
    }

    public override void OnNetworkDespawn()
    {
        if (IsOwner)
        {
            health.OnValueChanged -= OnHealthChanged; // 이벤트 핸들러 해제
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
    public void decHealthRpc() // 확실하게 RPC로 NetworkVariable 갱신
    {
        decHealth();
    }

    private void OnGUI() // GUI를 처리하는 이벤트 처리기
    {
        Vector3 offset = new Vector3(-3.0f, 4.0f, 0.0f);
        Vector3 pos = Camera.main.WorldToScreenPoint(transform.position + offset);
        Rect rect = new Rect(0, 0, 100, 50);
        rect.x = pos.x;
        rect.y = Screen.height - pos.y;
        // GUI style 사용
        GUIStyle style = new GUIStyle();
        style.richText = true; // HTML 태그 사용을 허용
        string text = $"<color=blue>{userId.Value}</color>: <color=red>{health.Value}</color>";
        GUI.Label(rect, text);
    }
}
