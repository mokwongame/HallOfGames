using UnityEngine;

public class GolemPhaseController : Enemy
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        Init();

    }

    // Update is called once per frame
    void Update()
    {
        if (MoveCheck(6f))
        {
            transform.Translate(m_speed * Time.deltaTime, 0, 0);
        }
    }
}
