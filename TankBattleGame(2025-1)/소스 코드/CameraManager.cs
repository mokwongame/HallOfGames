using Unity.Netcode;  // Unity Netcode for GameObjects 라이브러리를 사용하여 네트워크 기능 구현
using UnityEngine;    // Unity 엔진의 기본 기능을 사용하기 위한 네임스페이스

// 이 클래스는 네트워크 환경에서 카메라를 플레이어에 따라 움직이도록 관리함
public class CameraManager : NetworkBehaviour
{
    [SerializeField]
    private GameObject player;  // 추적할 플레이어 오브젝트 (에디터에서 할당하거나 런타임 중 찾음)

    public float sensitivity = 5f;  // 마우스 회전 감도
    private bool _bIsDragging;      // 현재 마우스를 드래그 중인지 여부
    private float _currentAngle;    // 카메라의 현재 회전 각도(Y축 기준)

    // 카메라의 거리 및 고도 설정값 (고정값)
    private const float Distance = 15f;     // 플레이어로부터 카메라까지의 거리
    private const float Height = 5f;        // 플레이어 기준 카메라 높이
    private const float FixedXAngle = 20f;  // 카메라의 X축(상하) 고정 각도

    // 매 프레임마다 호출되는 Unity 기본 함수
    private void Update()
    {
        // 아직 플레이어가 할당되지 않았다면 시도
        if (!player)
        {
            TryAssignPlayer();
            return;  // 플레이어가 없으면 카메라 업데이트 생략
        }

        HandleMouseInput();      // 마우스 입력 처리
        UpdateCameraPosition();  // 카메라 위치 및 회전 갱신
    }

    // 플레이어 오브젝트가 아직 할당되지 않은 경우 자동으로 찾아서 할당
    private void TryAssignPlayer()
    {
        // 네트워크 매니저 또는 스폰 매니저가 존재하지 않으면 중단
        if (!NetworkManager.Singleton || NetworkManager.Singleton.SpawnManager == null)
        {
            return;
        }

        // 현재 호스트(서버)라면 첫 번째 연결된 클라이언트의 PlayerObject를 가져옴
        if (NetworkManager.Singleton.IsHost)
        {
            if (NetworkManager.Singleton.ConnectedClients.Count > 0)
            {
                player = NetworkManager.Singleton.ConnectedClients[0].PlayerObject?.gameObject;
            }
        }
        // 클라이언트라면 자신의 로컬 PlayerObject를 가져옴
        else if (NetworkManager.Singleton.IsClient)
        {
            player = NetworkManager.Singleton.SpawnManager.GetLocalPlayerObject()?.gameObject;
        }

        // 기본적인 초기 카메라 회전 설정 (정면에서 위쪽을 향하는 각도)
        transform.rotation = Quaternion.Euler(80, 0, 0);
    }

    // 마우스 입력으로 카메라 회전 조작을 감지 및 처리
    private void HandleMouseInput()
    {
        // 마우스 왼쪽 버튼 클릭 시작
        if (Input.GetMouseButtonDown(0))
        {
            _bIsDragging = true;
        }

        // 마우스 왼쪽 버튼을 놓음
        if (Input.GetMouseButtonUp(0))
        {
            _bIsDragging = false;
        }

        // 마우스를 드래그하는 동안 X축 움직임에 따라 카메라 회전 각도를 변경
        if (_bIsDragging)
        {
            float mouseX = Input.GetAxis("Mouse X");  // 마우스의 수평 이동값
            _currentAngle += mouseX * sensitivity;     // 회전 각도에 누적
        }
    }

    // 플레이어 위치에 따라 카메라 위치 및 회전값을 조정
    private void UpdateCameraPosition()
    {
        // 플레이어 오브젝트가 없으면 실행하지 않음
        if (!player)
        {
            return;
        }

        // 고정된 X축 각도와 드래그로 변화된 Y축 각도를 적용한 회전 쿼터니언
        Quaternion rotation = Quaternion.Euler(FixedXAngle, _currentAngle, 0);

        // 회전 쿼터니언을 기준으로 플레이어 뒷부분(Distance 만큼) 위치 계산
        Vector3 offset = rotation * new Vector3(0, 0, -Distance);

        // 최종 카메라 위치 = 플레이어 위치 + 거리 보정 + 높이 보정
        transform.position = player.transform.position + offset + Vector3.up * Height;

        // 카메라의 회전을 계산된 회전값으로 설정
        transform.rotation = rotation;
    }
}
