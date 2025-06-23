using UnityEngine;
using Unity.Netcode;

// 탱크 포탑을 좌우로 회전시키는 클래스
public class RotTurret : NetworkBehaviour
{
    void Update()
    {
        // 오브젝트나 게임 매니저가 없으면 아무 작업도 하지 않음
        if (!gameObject || !BeginGameManager.Instance)
        {
            return;
        }

        int keyType = 0;  // 입력 상태: -1(왼쪽), 0(없음), 1(오른쪽)

        // 'J' 키가 눌리면 왼쪽으로 회전
        if (Input.GetKey(KeyCode.J))
        {
            keyType = -1;
        }
        // 'L' 키가 눌리면 오른쪽으로 회전
        else if (Input.GetKey(KeyCode.L))
        {
            keyType = 1;
        }

        // 회전 각도 계산: 속도 × 방향 × 시간
        float angleOff = keyType * BeginGameManager.Instance.RotSpeedTank * Time.deltaTime;

        // Y축을 기준으로 포탑 회전
        transform.Rotate(0.0f, angleOff, 0.0f);
    }
}
