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
    protected int moleScore;//두더지가 죽을 시에 주는 스코어
    [SerializeField] 
    protected int moleTime;//두더지가 죽을 시에 주는 스코어





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
