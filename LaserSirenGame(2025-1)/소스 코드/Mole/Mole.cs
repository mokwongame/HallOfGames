using System.Collections;
using UnityEngine;
using static UnityEngine.Rendering.DebugUI;

public class Mole : MonoBehaviour
{
    enum MoleType
    {
        CommonMole,
        BombMole,
        ArmoredMole
    };

    [SerializeField] MoleType type;

    protected int m_hp;
    [SerializeField] 
    protected int m_hpMax;

    [SerializeField] 
    protected int moleScore;//�δ����� ���� �ÿ� �ִ� ���ھ�
    [SerializeField] 
    protected int moleTime;//�δ����� ���� �ÿ� �ִ� ���ھ�





    public void GetDamage(int damage)
    {
        m_hp -= damage;
        if (type == MoleType.ArmoredMole)
        {
            Debug.Log(m_hp);
            GameManager.Instance.PlaySound("Shield");
        }


        if (m_hp <= 0)
        {
            GameManager.Instance.incScore(moleScore);
            GameManager.Instance.incTime(moleTime);
            if (type == MoleType.BombMole)
            {
                GameManager.Instance.PlaySound("Bomb");
            }
            Destroy(gameObject);
        }
    }

}
