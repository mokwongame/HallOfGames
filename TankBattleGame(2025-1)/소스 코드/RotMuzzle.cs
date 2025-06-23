using UnityEngine;
using Unity.Netcode;

// 탱크의 포신(총구)을 위아래로 조절하는 클래스
public class RotMuzzle : NetworkBehaviour
{
    private const float AngleMax = 0.0f;    // 포신이 들릴 수 있는 최대 각도 (0도, 정면)
    private const float AngleMin = -15.0f;  // 포신이 내려갈 수 있는 최소 각도 (-15도)

    // 매 프레임마다 호출
    void Update()
    {
        // 게임 오브젝트나 게임 매니저가 존재하지 않으면 작업 중단
        if (!gameObject || !BeginGameManager.Instance)
        {
            return;
        }

        int keyType = 0;  // 입력 상태: 1(위), -1(아래), 0(없음)

        // 'I' 키를 누르면 포신을 위로 올림
        if (Input.GetKey(KeyCode.I))
        {
            keyType = 1;
        }
        // 'K' 키를 누르면 포신을 아래로 내림
        else if (Input.GetKey(KeyCode.K))
        {
            keyType = -1;
        }

        // 현재 로컬 회전값을 가져옴 (0~360 범위)
        Vector3 rot = transform.localEulerAngles;

        // 입력에 따른 회전값 계산 (속도 * 방향 * 시간)
        float angleOff = -keyType * BeginGameManager.Instance.RotSpeedTank * Time.deltaTime;

        // 현재 X축 회전 각도 계산
        double xAngle = rot.x;
        xAngle += angleOff;

        // 유니티의 오일러 각은 0~360도로 표현되므로 180 초과 시 보정
        if (xAngle > 180)
        {
            xAngle -= 360;
        }

        // 회전 각도가 제한 범위 내에 있을 경우만 회전 적용
        if (xAngle >= AngleMin && xAngle <= AngleMax)
        {
            transform.Rotate(angleOff, 0.0f, 0.0f);  // X축 회전만 적용
        }
    }
}
