using System.Collections;
using UnityEngine;

public class CommonMole : Mole
{

    public float ymin = -2f;
    public float ymax = 2f;
    public float speed = 3.0f;
    public float ypos;

    void Start()
    {
        ypos = ymin;
        m_hp = m_hpMax;
        StartCoroutine("moveMole");
    }


    IEnumerator moveMole()
    {
        bool check = false;
        while (true)
        {
            ypos += speed * Time.deltaTime;
            if (ypos > ymax)
            {
                //처음 내려갈 때
                if (!check)
                {
                    yield return new WaitForSeconds(Random.Range(0.5f, 2f));

                    check = true;
                }
                speed = -speed;
            }
            else if (ypos < ymin)
            {
                break;
            }
            Vector3 pos = transform.position;
            pos.y = ypos;
            transform.position = pos;
            yield return null;
        }

        Destroy(gameObject);

    }
}
