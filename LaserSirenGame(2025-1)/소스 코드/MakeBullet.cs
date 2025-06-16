using System.Collections;
using UnityEngine;

public class MakeBullet : MonoBehaviour
{


    private GameObject axisX;
    private GameObject axisY;

    [SerializeField] GameObject bullet;
    [SerializeField] float m_delay; //딜레이
    bool isAttacking;
    float m_lastClickTime = 0;//마지막으로 클릭한 시간

    private void Start()
    {
        axisX = GameObject.Find("AxisX");
        axisY = GameObject.Find("AxisY");
    }


    void Update()
    {
        if (Input.GetMouseButton(0))
        {
            float currentTime = Time.time;// 클릭한 시간
            float clickInterval = currentTime - m_lastClickTime; // 이전 클릭 시간과 간격
            if (m_delay < clickInterval)// 딜레이가 끝나고 클릭했는가?
            {
                m_lastClickTime = currentTime;//마지막으로 클릭한 시간
                OnMouseButtonDown();
            }
            else { } // 딜레이중 클릭한경우 발사X
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

                //Debug.Log($"위치 : {pos}");
                Debug.DrawRay(ray.origin, ray.direction * 1000);

                pos = transform.parent.position + (transform.parent.up * axisY.transform.localPosition.y); //카메라에서 axisY만큼 떨어진 위치
                pos.x = Mathf.Sign(axisX.transform.position.x) * 3.07f;//조준점 xmax

                GameObject temp = Instantiate(bullet, pos, Quaternion.LookRotation(pos - hit.point));
                GameManager.Instance.PlaySound("FireBullet");

            }

            yield return new WaitForSeconds(m_delay);

        }
    }
}
