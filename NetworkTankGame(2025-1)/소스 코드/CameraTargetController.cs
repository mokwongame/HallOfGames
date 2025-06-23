/*
 * [CameraTargetController]
 * ��ũ�� �����Ӱ� ���⿡ ���� ī�޶� Ÿ���� ��ġ�� ȸ�� ������ �ڵ����� �����ϴ� ��ũ��Ʈ
 * CameraTarget�� ��ġ�� ��ũ�� Transform�� �������� �����Ǹ�, ��ũ�� �Ĺ��� �ٶ� �� ī�޶� �������� �����
 * Cinemachine�� ����Ͽ� CameraTarget�� ��ġ�� ���󰡰� �ε巯�� ī�޶� �������� ������
 */
using UnityEngine;


public class CameraTargetController : MonoBehaviour
{
    [SerializeField] private Vector3 vOffset = new Vector3(0.0f, 10.0f, -15.0f); //ī�޶�� ��ũ ������ ������ ��ġ

    private Transform tankTransform = null; //��ũ�� Transform ������Ʈ
    private bool isRearView = false;        //�Ĺ� �� ����(��ũ�� ���� ������ �Ѿ�� �Ĺ� ��� ��ȯ�ϱ� ����)

    public void f_SetTankTransform(Transform target)
    {
        tankTransform = target;
    }

    /*
     * LateUpdate�� ��� Update�� ���� �Ŀ� ȣ��ǹǷ� ī�޶� ���� ������ ������ ����
     */
    private void LateUpdate()
    {
        if (tankTransform == null) return;

        f_UpdateCamOffset();    //��ũ�� ȸ�� ������ ���� ī�޶� �������� ������Ʈ
        f_FollowTank();         //��ũ�� ��ġ�� ���� ī�޶� ��ġ�� ������Ʈ
    }

    private void f_UpdateCamOffset()
    {
        float fYAngle = tankTransform.eulerAngles.y; //��ũ�� Y�� ȸ�� ���� ��������

        //�Ĺ� ����: ���� �������� ���� 90~270�� ��ġ(���� �������� �߾��� ���������� ��, ȸ�� ���ؼ� ����� ��� ��ȯ ��Ŵ)
        bool isBehind = (fYAngle > 90.0f && fYAngle < 270.0f);

        if (!isRearView && isBehind) //��ũ�� �Ĺ��� �ٶ󺸰� �ְ� ���� �Ĺ� �䰡 �ƴ� ���
        {
            vOffset.z = Mathf.Abs(vOffset.z); //Z�� �������� ����� �����Ͽ� �Ĺ� ��� ��ȯ
            isRearView = true;
        }
        else if (isRearView && !isBehind) //��ũ�� �Ĺ��� �ٶ��� �ʰ� �ְ� ���� �Ĺ� ���� ���
        {
            vOffset.z = -Mathf.Abs(vOffset.z); //Z�� �������� ������ �����Ͽ� ���� ��� ��ȯ
            isRearView = false;
        }
    }

    /// <summary> ��ũ�� ��ġ�� ���� ī�޶� ��ġ�� ������Ʈ�ϴ� �޼ҵ� </summary>
    private void f_FollowTank() 
    {
        transform.position = tankTransform.position + vOffset;
    }
}