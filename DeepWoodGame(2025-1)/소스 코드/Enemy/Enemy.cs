using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    //abstract
    enum EnemyType
    {
        Rat,
        SpikedSlim,
        Bat,
        GolemPhase,
    };

    [SerializeField] LayerMask m_layerMask;//플레이어
    [SerializeField] EnemyType type;

    [SerializeField]
    protected int m_hp;
    [SerializeField]
    protected int m_hpMax;
    [SerializeField]
    protected float m_speed;

    [SerializeField] 
    int m_attackPower;//공격력
    [SerializeField] 
    float m_delay; //공격 딜레이
    [SerializeField]
    protected int enemyScore;//죽을 시에 주는 스코어

    [SerializeField] Transform m_bulletPoint;
    [SerializeField] Transform m_bullet;

    [SerializeField] Animator enemyAnim;


    protected bool isDeath = false;
    protected void Init()
    {
        m_hp = m_hpMax;
        enemyAnim = gameObject.GetComponent<Animator>();
    }

    public void GetDamage(int damage)
    {
        if(isDeath)
            return;

        GameManager.Instance.PlaySound("EnemyHit");
        m_hp -= damage;
        if (m_hp <= 0)
        {
            isDeath = true;
            //애니매이션 종료 후 삭제
            StartCoroutine(Death());
        }
        else
        {
            enemyAnim.SetTrigger("Hit");
        }
    }

    IEnumerator Death()
    {
        GameManager.Instance.incScore(enemyScore);
        enemyAnim.SetTrigger("Death");
        yield return new WaitForSeconds(1);
        Destroy(gameObject);
    }




    float m_lastClickTime = 0;
    protected bool MoveCheck(float checkDis)
    {
        //dis만큼 이동 할 때 그 사이에Player가 있을 경우
        Vector2 origin = transform.position;
        Vector2 dir = Vector2.down;
        origin.x += (transform.localScale.x / 2) - (checkDis / 2);
        origin.y += 1;
        Debug.DrawRay(origin, dir * 5, Color.red);
        bool hit = Physics2D.OverlapBox(origin,new Vector2(checkDis, 10) ,0 , m_layerMask);


        if (hit)
        {
            if (isDeath)
                return false;

            float currentTime = Time.time;// 피격한 시간
            float clickInterval = currentTime - m_lastClickTime; // 이전 공격 시간과 간격
            if (m_delay < clickInterval)// 딜레이가 끝나고 공격했는가?
            {
                m_lastClickTime = currentTime;//마지막으로 공격한 시간
                GameManager.Instance.PlaySound("EnemyAttack");
                
                switch (type)
                {
                    case EnemyType.Rat:
                        PlayerController.Instance.GetDamage(m_attackPower);
                        enemyAnim.SetTrigger("Attack");
                        break;
                    case EnemyType.SpikedSlim:
                        PlayerController.Instance.GetDamage(m_attackPower);
                        enemyAnim.SetTrigger("Attack");
                        break;
                    case EnemyType.Bat:
                        PlayerController.Instance.GetDamage(m_attackPower);
                        enemyAnim.SetTrigger("Attack");
                        break;
                    case EnemyType.GolemPhase:
                        enemyAnim.SetTrigger("Attack 3");
                        Instantiate(m_bullet, m_bulletPoint.position, Quaternion.identity);
                        break;
                }
            }
            return false;
        }
        return true;
    }

}
