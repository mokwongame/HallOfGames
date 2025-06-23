using Unity.Collections;
using Unity.Netcode;
using UnityEngine;

public class PlayerHealthNet : NetworkBehaviour
{
    private const int MaxHealth = 100;  // 최대 체력

    private const int GUIWidth = 100;   // GUI 표시 가로 크기
    private const int GUIHeight = 50;   // GUI 표시 세로 크기

    // 네트워크 변수로 체력 관리, 모두가 읽을 수 있고 소유자만 쓸 수 있음
    public NetworkVariable<int> health = new(0, NetworkVariableReadPermission.Everyone, NetworkVariableWritePermission.Owner);

    // 네트워크 변수로 사용자 ID 관리, 모두가 읽을 수 있고 소유자만 쓸 수 있음
    public NetworkVariable<FixedString64Bytes> userId = new(default, NetworkVariableReadPermission.Everyone, NetworkVariableWritePermission.Owner);

    private Camera _mainCamera;         // 메인 카메라 참조

    private void Start()
    {
        // 메인 카메라 할당
        _mainCamera = Camera.main;
    }

    // 네트워크에 오브젝트가 스폰될 때 호출
    public override void OnNetworkSpawn()
    {
        // 소유자 클라이언트라면 초기 체력과 사용자 ID 설정, UI 갱신
        if (IsOwner)
        {
            health.Value = MaxHealth;
            userId.Value = BeginGameManager.Instance.UserId;
            UiManagerTank.Instance.UpdateUIInfo(health.Value);
        }

        base.OnNetworkSpawn();
    }

    // 체력 감소 처리 함수 (로컬 소유자에서 호출)
    private void DecHealth(int damage)
    {
        // 현재 체력에서 데미지를 뺌
        int curValue = health.Value - damage;

        // UI 갱신 (체력 표시)
        UiManagerTank.Instance.UpdateUIInfo(curValue);

        // 네트워크 변수 체력 갱신
        health.Value = curValue;

        // 체력이 0 이하가 되면 서버에 리스폰 요청 및 오브젝트 파괴 호출
        if (curValue <= 0)
        {
            RequestRespawnServerRpc(OwnerClientId);
            DestroyEventRpc();
        }
    }

    // 서버 RPC: 리스폰 요청, 클라이언트 ID를 서버에 전달
    [ServerRpc(RequireOwnership = false)]
    private void RequestRespawnServerRpc(ulong clientId)
    {
        // 서버의 InGameManager에 리스폰 처리 위임
        InGameManager.Instance.HandleRespawn(clientId);
    }

    // 서버에 보내는 RPC: 네트워크 오브젝트 언스폰(파괴) 요청
    [Rpc(SendTo.Server)]
    private void DestroyEventRpc()
    {
        NetworkObject.Despawn();
    }

    // 소유자 클라이언트에 보내는 RPC: 체력 감소 처리 함수 호출
    [Rpc(SendTo.Owner)]
    public void DecHealthRpc(int damage)
    {
        DecHealth(damage);
    }

    // 유니티 OnGUI 함수: 화면에 플레이어 체력과 사용자 ID를 표시
    private void OnGUI()
    {
        if (!_mainCamera)
        {
            return;
        }

        // 캐릭터 머리 위로 2 단위만큼 올린 위치
        Vector3 offset = new Vector3(0f, 2f, 0f);

        // 월드 좌표를 스크린 좌표로 변환
        Vector3 pos = _mainCamera.WorldToScreenPoint(transform.position + offset);

        // 화면 좌표 기준으로 표시할 영역 설정
        Rect rect = new Rect(0, 0, GUIWidth, GUIHeight)
        {
            x = pos.x,
            // GUI는 좌측 하단이 (0,0)이므로 y축 보정 필요
            y = Screen.height - pos.y
        };

        // 표시할 텍스트: 사용자 ID와 체력 값
        string text = $"{userId.Value} : {health.Value}";

        // GUI 레이블로 화면에 표시
        GUI.Label(rect, text);
    }
}
