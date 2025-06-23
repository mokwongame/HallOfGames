/*
 * 탱크를 특정위치에서 따라다니는 카메라 스크립트
 * Cinemachine을 사용하여 탱크의 특정 위치에서 추적하도록 설계함
 */

using UnityEngine;
using Unity.Netcode;
using Unity.Cinemachine;

public class FollowQuarterCamera : NetworkBehaviour
{
    //네트워크 오브젝트가 생성될 때 호출되는 메소드를 오버라이드
    //탱크가 네트워크에 스폰될 때 카메라를 설정하기 위함(오류 방지)
    public override void OnNetworkSpawn() 
    {
        if (!IsOwner) return; //오너가 아닌 경우 리턴

        GameObject cameraTargetObj = GameObject.Find("CameraTarget"); //CameraTarget 오브젝트 찾기
        if (cameraTargetObj == null) return;

        CameraTargetController camController = cameraTargetObj.GetComponent<CameraTargetController>(); //CameraTargetController 컴포넌트 찾기

        if (camController != null)
        {
            camController.f_SetTankTransform(transform); //탱크의 Transform을 CameraTargetController에 설정
        }

        CinemachineCamera trackingCam = GameObject.Find("VirtualCam_Player")?.GetComponent<CinemachineCamera>();
        
        if (trackingCam != null)
        {
            trackingCam.Follow = cameraTargetObj.transform; //카메라의 추적 대상 설정(CameraTarget)
            trackingCam.LookAt = transform;                 //카메라가 바라볼 대상 설정(탱크)
        }
    }
}