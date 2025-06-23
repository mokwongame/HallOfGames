using System.Collections;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    [SerializeField] ParticleSystem m_hitEffect;//몬스터가 피격당할때
    [SerializeField] float m_bulletSpeed;

    bool m_isDestroy = false;

    void Start()
    {
        Vector3 mousePos;
        mousePos = GetMouseWorldPosition();
        transform.rotation = Quaternion.Euler(0, 0, (Mathf.Atan2(mousePos.y - transform.position.y, mousePos.x - transform.position.x) * Mathf.Rad2Deg) - 90);
        Invoke("BulletDestroy", 3);
    }

    // Update is called once per frame
    void Update()
    {
        if (m_isDestroy)
            return;



        transform.Translate(Vector2.up * m_bulletSpeed * Time.deltaTime);

        RaycastHit2D[] m_hits = Physics2D.RaycastAll(transform.position, transform.up, m_bulletSpeed * Time.deltaTime, 1 << LayerMask.NameToLayer("Enemy"));
        foreach (var hit in m_hits)
        {
            if (hit)
            {
                //피격된 오브젝트
                if (hit.transform.GetComponent<Enemy>() != null)
                {
                    m_isDestroy = true;
                    Enemy enemy = hit.transform.GetComponent<Enemy>();
                    enemy.GetDamage(1);
                    StartCoroutine(HitEffect());
                    return;
                }
            }
        }
    }
    private Vector3 GetMouseWorldPosition()
    {
        // 마우스 위치를 월드 좌표로 변환
        Vector3 mousePos = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y,
                                                                            -Camera.main.transform.position.z));//카메라부터의 거리
        return mousePos;
    }

    
    private IEnumerator HitEffect()
    {
        //파티클 시작
        transform.GetChild(0).gameObject.SetActive(false);
        ParticleSystem temp = Instantiate(m_hitEffect, transform.position, transform.rotation);
        temp.Play();
        yield return new WaitForSeconds(temp.main.duration);
        //파티클 종료

        temp.Stop();

        Destroy(temp.gameObject);
        BulletDestroy();
    }
    void BulletDestroy()
    {
        Destroy(gameObject);
    }
}
