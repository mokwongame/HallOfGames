using UnityEngine;

public class BatController : Enemy
{
    bool rdCheck=false;

    void Start()
    {
        Init();

    }

    void Update()
    {

        if (isDeath && !rdCheck)//���� �� �������� ���
        {
            rdCheck = true;
            gameObject.AddComponent<Rigidbody2D>();
        }
        if (MoveCheck(0.5f))
        {
            transform.Translate(m_speed * Time.deltaTime, 0, 0);
        }
    }
}
