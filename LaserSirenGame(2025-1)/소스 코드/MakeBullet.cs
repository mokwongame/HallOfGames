using System.Collections;
using UnityEngine;

public class MakeBullet : MonoBehaviour
{


    private GameObject axisX;
    private GameObject axisY;

    [SerializeField] GameObject bullet;
    [SerializeField] float m_delay; //������
    bool isAttacking;
    float m_lastClickTime = 0;//���������� Ŭ���� �ð�

    private void Start()
    {
        axisX = GameObject.Find("AxisX");
        axisY = GameObject.Find("AxisY");
    }


    void Update()
    {
        if (Input.GetMouseButton(0))
        {
            float currentTime = Time.time;// Ŭ���� �ð�
            float clickInterval = currentTime - m_lastClickTime; // ���� Ŭ�� �ð��� ����
            if (m_delay < clickInterval)// �����̰� ������ Ŭ���ߴ°�?
            {
                m_lastClickTime = currentTime;//���������� Ŭ���� �ð�
                OnMouseButtonDown();
            }
            else { } // �������� Ŭ���Ѱ�� �߻�X
        }
        if (Input.GetMouseButtonUp(0))
        {
            OnMouseButtonUp();
        }

        FireMissile();
    }

    private void OnMouseButtonDown()
    {
        isAttacking = true;
    }
    private void OnMouseButtonUp()
    {
        isAttacking = false;

    }


    void FireMissile()
    {
        if (isAttacking)
        {
            if (m_bulletFiring == null)
            {
                m_bulletFiring = BulletFiring();
                StartCoroutine(m_bulletFiring);
            }
        }
        else
        {
            if (m_bulletFiring != null)
            {
                StopCoroutine(m_bulletFiring);
                m_bulletFiring = null;
            }
        }
    }


    IEnumerator m_bulletFiring;

    IEnumerator BulletFiring()
    {
        while (true)
        {
            Vector3 pos = new Vector3(axisX.transform.position.x,
                                    axisY.transform.position.y,
                                    axisY.transform.position.z);

            RaycastHit hit;
            Ray ray = new Ray(transform.parent.position, (pos - transform.parent.position).normalized);
            //Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            if (Physics.Raycast(ray, out hit, 100f))
            {

                //Debug.Log($"��ġ : {pos}");
                Debug.DrawRay(ray.origin, ray.direction * 1000);

                pos = transform.parent.position + (transform.parent.up * axisY.transform.localPosition.y); //ī�޶󿡼� axisY��ŭ ������ ��ġ
                pos.x = Mathf.Sign(axisX.transform.position.x) * 3.07f;//������ xmax

                GameObject temp = Instantiate(bullet, pos, Quaternion.LookRotation(pos - hit.point));
                GameManager.Instance.PlaySound("FireBullet");

            }

            yield return new WaitForSeconds(m_delay);

        }
    }
}
