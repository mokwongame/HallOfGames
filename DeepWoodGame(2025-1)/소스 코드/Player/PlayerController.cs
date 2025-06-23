using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.SocialPlatforms.Impl;

public class PlayerController : MonoBehaviour
{
    public static PlayerController Instance;

    Rigidbody2D m_playerRigid;
    BoxCollider2D m_boxCollider;
    SpriteRenderer m_spriteRenderer;
    Animator m_playerAnim;

    [SerializeField] Transform m_transGroundCheck;
    [SerializeField] LayerMask m_layerGround;


    [SerializeField] float m_fMoveSpeed;
    [SerializeField] float m_fJumpPower;


    [SerializeField] int m_hp;
    [SerializeField] int m_maxHp;

    public int Hp => m_hp;
    public int MaxHp => m_maxHp;

    Vector2 m_vecMoveDir = Vector2.zero;

    bool m_isRun = false;
    bool m_isJump = false;

    [SerializeField] Transform m_attackRange;
    [SerializeField] float m_delay; //공격 딜레이
    float m_lastClickTime = 0;//마지막으로 클릭한 시간

    [SerializeField] Transform m_bulletPoint;
    [SerializeField] Transform m_bullet;

    bool isDeath = false;


    enum E_KEY_H_ARROW
    {
        None,
        Left,
        Right,
    }
    private void Awake()
    {
        m_hp = m_maxHp;
        m_playerRigid = gameObject.GetComponent<Rigidbody2D>();
        m_boxCollider = gameObject.GetComponent<BoxCollider2D>();
        m_spriteRenderer = gameObject.GetComponent<SpriteRenderer>();
        m_playerAnim = gameObject.GetComponent<Animator>();


        Instance = this;
    }
   

    void Update()
    {
        if(isDeath)
            return;

        GetInput();
        Move();
        Jump();
    }
    void GetInput()
    {
        m_isRun = false;
        switch (GetHorizontalKeyState(KeyCode.A, KeyCode.D))
        {
            case E_KEY_H_ARROW.Left:
                m_vecMoveDir = Vector2.left;
                break;
            case E_KEY_H_ARROW.Right:
                m_vecMoveDir = Vector2.right;
                m_isRun = true;
                break;
            case E_KEY_H_ARROW.None:
                m_vecMoveDir = Vector2.zero;
                break;

        }

        m_playerAnim.SetBool("IsRunning", m_isRun);

        //원거리
        if (Input.GetMouseButtonDown(1))
        {
            float currentTime = Time.time;// 클릭한 시간
            float clickInterval = currentTime - m_lastClickTime; // 이전 클릭 시간과 간격
            if (m_delay < clickInterval)// 딜레이가 끝나고 클릭했는가?
            {
                m_lastClickTime = currentTime;//마지막으로 클릭한 시간

                GameManager.Instance.PlaySound("Bullet");
                Instantiate(m_bullet, m_bulletPoint.position, Quaternion.identity);
            }
        }
        //근거리
        if (Input.GetMouseButtonDown(0))
        {
            float currentTime = Time.time;// 클릭한 시간
            float clickInterval = currentTime - m_lastClickTime; // 이전 클릭 시간과 간격
            if (m_delay < clickInterval)// 딜레이가 끝나고 클릭했는가?
            {
                m_lastClickTime = currentTime;//마지막으로 클릭한 시간

                GameManager.Instance.PlaySound("PlayerAttack");
                m_playerAnim.SetTrigger("Attack");
                Collider2D[] collider2Ds = Physics2D.OverlapBoxAll(m_attackRange.position, m_attackRange.localScale, 0);
                foreach (Collider2D collider in collider2Ds)
                {
                    //피격된 오브젝트
                    if (collider.gameObject.GetComponent<Enemy>() != null)
                    {
                        Enemy enemy = collider.gameObject.GetComponent<Enemy>();
                        StartCoroutine(GetEnemyDamage(enemy));
                    }
                }
            }

            
        }

    }

    IEnumerator GetEnemyDamage(Enemy enemy)
    {
        yield return new WaitForSeconds(0.5f);
        enemy.GetDamage(2);
    }

    E_KEY_H_ARROW m_eLastKey = E_KEY_H_ARROW.None;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="left">왼쪽 키</param>
    /// <param name="right">오른쪽 키</param>
    /// <returns></returns>
    E_KEY_H_ARROW GetHorizontalKeyState(KeyCode left, KeyCode right)
    {
        //좌우로 누른키가 없는경우
        if (!(Input.GetKey(left) || Input.GetKey(right)))
            return E_KEY_H_ARROW.None;

        //마지막으로 누른 키 찾기
        if (Input.GetKeyDown(left))
            m_eLastKey = E_KEY_H_ARROW.Left;
        if (Input.GetKeyDown(right))
            m_eLastKey = E_KEY_H_ARROW.Right;

        //키를 땠을때 다른키를 누르고있는가?
        if (Input.GetKeyUp(left) && Input.GetKey(right))
            m_eLastKey = E_KEY_H_ARROW.Right;
        if (Input.GetKeyUp(right) && Input.GetKey(left))
            m_eLastKey = E_KEY_H_ARROW.Left;

        return m_eLastKey;
    }

    void Move()
    {
        transform.Translate(m_fMoveSpeed * m_vecMoveDir * Time.deltaTime);
    }

    void Jump()
    {

        if (IsGrounded())
        {
            if (Input.GetKey(KeyCode.Space))
            {
                m_playerAnim.SetBool("IsJumpUp", true);
                m_playerRigid.linearVelocityY = 0;
                m_playerRigid.AddForce(Vector2.up * m_fJumpPower, ForceMode2D.Impulse);
            }
        }
    }


    public bool IsGrounded()
    {
        bool isGroundCheck;
        isGroundCheck = Physics2D.OverlapBox(m_transGroundCheck.position,
                        m_transGroundCheck.localScale, 0, m_layerGround);

        if(!isGroundCheck && m_playerRigid.linearVelocityY <= 0.001)
        {
            m_playerAnim.SetBool("IsJumpUp", false);
            m_playerAnim.SetBool("IsJumpDown", true);
        }
        else
        {
            m_playerAnim.SetBool("IsJumpDown", false);
        }

        return isGroundCheck && m_playerRigid.linearVelocityY <= 0.001;
    }

    bool isInvincibility = false;
    public void GetDamage(int damage)
    {
        if (isInvincibility || isDeath)
            return;
        GameManager.Instance.PlaySound("PlayerHit");
        m_hp -= damage;
        GameManager.Instance.updateHp();
        StartCoroutine(Hit());
        m_playerRigid.AddForce(new Vector2(-8,10), ForceMode2D.Impulse);
        if (m_hp <= 0)
        {
            isDeath = true;
            m_playerAnim.SetTrigger("Death");
            Invoke("EndScene", 3);
            


        }
    }

    IEnumerator Hit()
    {
        isInvincibility = true;
        Color c = m_spriteRenderer.color; // 현재 색상 복사
        c.a = 0.5f;                    // 알파값 변경
        m_spriteRenderer.color = c;       // 색상 적용
        yield return new WaitForSeconds(0.4f);
        c.a = 1f;
        m_spriteRenderer.color = c;

        isInvincibility = false;
    }
    public void EndScene()
    {
        PlayerPrefs.SetInt("Score", GameManager.Instance.score);
        SceneManager.LoadScene("EndScene");
    }

}
