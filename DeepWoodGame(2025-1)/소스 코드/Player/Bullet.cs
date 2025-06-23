using System.Collections;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    [SerializeField] ParticleSystem m_hitEffect;//���Ͱ� �ǰݴ��Ҷ�
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
                //�ǰݵ� ������Ʈ
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
        // ���콺 ��ġ�� ���� ��ǥ�� ��ȯ
        Vector3 mousePos = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y,
                                                                            -Camera.main.transform.position.z));//ī�޶������ �Ÿ�
        return mousePos;
    }

    
    private IEnumerator HitEffect()
    {
        //��ƼŬ ����
        transform.GetChild(0).gameObject.SetActive(false);
        ParticleSystem temp = Instantiate(m_hitEffect, transform.position, transform.rotation);
        temp.Play();
        yield return new WaitForSeconds(temp.main.duration);
        //��ƼŬ ����

        temp.Stop();

        Destroy(temp.gameObject);
        BulletDestroy();
    }
    void BulletDestroy()
    {
        Destroy(gameObject);
    }
}
