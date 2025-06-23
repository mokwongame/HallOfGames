//�÷��̾ SafeZone(���� ����)�� ������� üũ�ϴ� ��ũ��Ʈ
using System.Globalization;
using UnityEngine;
using Unity.Netcode;

public class PlayerOutZoneDetector : NetworkBehaviour
{
    private Collider zoneCollider = null;   //SafeZone�� �ݶ��̴�
    private float fCheckInterval = 1.0f;    //üũ ���� (��)
    private float fTimer = 0.0f;            //üũ�� ���� Ÿ�̸�

    void Start()
    {
        GameObject zoneObject = GameObject.FindGameObjectWithTag("SafeZone");

        if (zoneObject != null)
        {
            zoneCollider = zoneObject.GetComponent<Collider>();
        }

        if (zoneCollider == null)
        {
            Debug.LogError("SafeZone �ݶ��̴��� �����ϴ�. �±װ� 'SafeZone'�̰� �ݶ��̴� ������Ʈ�� �ִ��� Ȯ���ϼ���.");
        }
    }

    void Update()
    {
        if (!IsOwner) return;
        if (zoneCollider == null) return; //SafeZone �ݶ��̴��� ������ üũ���� ����

        fTimer += Time.deltaTime;

        if (fTimer >= fCheckInterval) //1�� ���� üũ, �ý��� �ڿ� ����
        {
            fTimer = 0.0f;
            
            f_CheckOutOfZone(); //SafeZone �ۿ� �ִ��� �˻� �� ó��
        }
    }

    /// <summary> SafeZone �ۿ� �ִ��� �˻� �� ó���ϴ� �޼ҵ� </summary>
    void f_CheckOutOfZone()
    {
        Vector3 pos = transform.position;

        if (!zoneCollider.bounds.Contains(pos))
        {
            Debug.Log("�÷��̾ ���� ���� �ۿ� ����");

            PlayerHealthNet health = GetComponent<PlayerHealthNet>();
            
            if (health != null)
            {
                health.health.Value = 0;

                SoundManager.Instance.f_PlaySFX(SoundName.SFX_GameOver, 0.7f); //���ӿ��� ȿ���� ���

                Invoke("f_GameOver", 2.0f); //2�� �� ���ӿ��� ó��
            }
        }
    }

    /// <summary> nvoke�� ���� ȣ��Ǵ� ���ӿ��� ó�� �޼ҵ� </summary>
    void f_GameOver()
    {
        FlowManager.Instance.f_OpenScene(SceneName.EndScene); //���ӿ��� ������ ��ȯ
    }
}