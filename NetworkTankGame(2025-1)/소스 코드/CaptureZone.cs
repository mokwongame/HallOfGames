/*
 * [���� ��� ��ũ��Ʈ]
 * ���� ������ �÷��̾ 10�� �̻� �ӹ����� ������ ü���� 20 ���ҽ�Ű�� ����� �����ϴ� ��ũ��Ʈ
 */
using UnityEngine;
using Unity.Netcode;


public class CaptureZone : NetworkBehaviour
{
    private float fStayTime = 0.0f;         //�÷��̾ �������� �ӹ��� �ð�
    private float fCaptureDuration = 5.0f;  //���ɿ� �ʿ��� �ð�

    private bool bCaptured = false; //���� ����

    private void OnTriggerStay(Collider other)
    {
        NetworkObject netObject = other.GetComponent<NetworkObject>();

        /*
         * �̹� ���ɵ� ������ ����
         * �÷��̾ �ƴ� ��� ����
         * ������ ���������� ó���ؾ��ϹǷ� ���������� ����
         */
        if (bCaptured) return;
        if (!IsServer) return; 
        if (!other.CompareTag("Player")) return; 
        if (netObject == null || !netObject.IsOwner) return; //��Ʈ��ũ ������Ʈ�� ���ų� ���ʰ� �ƴ� ��� ����

        fStayTime += Time.deltaTime; //�÷��̾ ������ �ӹ��� �ð� ������Ŵ

        if (fStayTime >= fCaptureDuration) //���� �ð��� �����ߴ��� Ȯ��
        {
            bCaptured = true; //�ߺ� ���� ����
            Debug.Log("���� �Ϸ�!");
            SoundManager.Instance.f_PlaySFX(SoundName.SFX_AreaSecured, 0.7f); //���� �Ϸ� ȿ���� ���

            GameManager.Instance.f_DamageAllExcept(other.gameObject, 20); //GameManager�� ���� ���� �÷��̾��� ü���� ���ҽ�Ŵ
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (!other.CompareTag("Player")) return; //�÷��̾ �ƴ� ��� ����

        fStayTime = 0.0f; //Ÿ�̸� �ʱ�ȭ
    }
}
