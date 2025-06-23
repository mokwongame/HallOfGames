/*
 * [CameraTargetController]
 * 탱크의 움직임과 방향에 따라 카메라 타겟의 위치와 회전 방향을 자동으로 조정하는 스크립트
 * CameraTarget의 위치는 탱크의 Transform을 기준으로 설정되며, 탱크가 후방을 바라볼 때 카메라 오프셋이 변경됨
 * Cinemachine을 사용하여 CameraTarget의 위치를 따라가고 부드러운 카메라 움직임을 구현함
 */
using UnityEngine;


public class CameraTargetController : MonoBehaviour
{
    [SerializeField] private Vector3 vOffset = new Vector3(0.0f, 10.0f, -15.0f); //카메라와 탱크 사이의 오프셋 위치

    private Transform tankTransform = null; //탱크의 Transform 컴포넌트
    private bool isRearView = false;        //후방 뷰 여부(탱크가 일정 각도를 넘어가면 후방 뷰로 전환하기 위함)

    public void f_SetTankTransform(Transform target)
    {
        tankTransform = target;
    }

    /*
     * LateUpdate는 모든 Update가 끝난 후에 호출되므로 카메라가 덜덜 떨리는 현상을 방지
     */
    private void LateUpdate()
    {
        if (tankTransform == null) return;

        f_UpdateCamOffset();    //탱크의 회전 각도에 따라 카메라 오프셋을 업데이트
        f_FollowTank();         //탱크의 위치에 따라 카메라 위치를 업데이트
    }

    private void f_UpdateCamOffset()
    {
        float fYAngle = tankTransform.eulerAngles.y; //탱크의 Y축 회전 각도 가져오기

        //후방 판정: 앞을 기준으로 양쪽 90~270도 위치(원을 기준으로 중앙을 가로지르는 선, 회전 기준선 벗어나는 경우 전환 시킴)
        bool isBehind = (fYAngle > 90.0f && fYAngle < 270.0f);

        if (!isRearView && isBehind) //탱크가 후방을 바라보고 있고 현재 후방 뷰가 아닌 경우
        {
            vOffset.z = Mathf.Abs(vOffset.z); //Z축 오프셋을 양수로 설정하여 후방 뷰로 전환
            isRearView = true;
        }
        else if (isRearView && !isBehind) //탱크가 후방을 바라보지 않고 있고 현재 후방 뷰인 경우
        {
            vOffset.z = -Mathf.Abs(vOffset.z); //Z축 오프셋을 음수로 설정하여 전방 뷰로 전환
            isRearView = false;
        }
    }

    /// <summary> 탱크의 위치에 따라 카메라 위치를 업데이트하는 메소드 </summary>
    private void f_FollowTank() 
    {
        transform.position = tankTransform.position + vOffset;
    }
}