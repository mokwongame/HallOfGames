/*
 * ��ũ�� Ư����ġ���� ����ٴϴ� ī�޶� ��ũ��Ʈ
 * Cinemachine�� ����Ͽ� ��ũ�� Ư�� ��ġ���� �����ϵ��� ������
 */

using UnityEngine;
using Unity.Netcode;
using Unity.Cinemachine;

public class FollowQuarterCamera : NetworkBehaviour
{
    //��Ʈ��ũ ������Ʈ�� ������ �� ȣ��Ǵ� �޼ҵ带 �������̵�
    //��ũ�� ��Ʈ��ũ�� ������ �� ī�޶� �����ϱ� ����(���� ����)
    public override void OnNetworkSpawn() 
    {
        if (!IsOwner) return; //���ʰ� �ƴ� ��� ����

        GameObject cameraTargetObj = GameObject.Find("CameraTarget"); //CameraTarget ������Ʈ ã��
        if (cameraTargetObj == null) return;

        CameraTargetController camController = cameraTargetObj.GetComponent<CameraTargetController>(); //CameraTargetController ������Ʈ ã��

        if (camController != null)
        {
            camController.f_SetTankTransform(transform); //��ũ�� Transform�� CameraTargetController�� ����
        }

        CinemachineCamera trackingCam = GameObject.Find("VirtualCam_Player")?.GetComponent<CinemachineCamera>();
        
        if (trackingCam != null)
        {
            trackingCam.Follow = cameraTargetObj.transform; //ī�޶��� ���� ��� ����(CameraTarget)
            trackingCam.LookAt = transform;                 //ī�޶� �ٶ� ��� ����(��ũ)
        }
    }
}