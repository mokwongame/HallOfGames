using UnityEngine;

public class SpikedSlimController : Enemy
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        Init();

    }

    // Update is called once per frame
    void Update()
    {
        if (MoveCheck(0.1f))
        {
            transform.Translate(m_speed * Time.deltaTime, 0, 0);
        }

    }
}
