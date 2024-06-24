using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovePotAct : MonoBehaviour
{
    float speed = 5.0f;
    float ymin = -4.5f;
    float ymax = 4.5f;
    int dir = 0;    // Pot�� �����̴� ����
    int changeDirSize = 120;

    // Start is called before the first frame update
    void Start()
    {
        // dir == 1: ����, dir == -1: �Ʒ���, dir == 0: ����
        dir = Random.Range(-1, 2); // -1, 0, 1(=2-1)
    }

    // Update is called once per frame
    void Update()
    {
        int rand = Random.Range(0, changeDirSize);
        if (rand == 0) dir *= -1; // changeDirSize�� �� �� ���� ���� ��ȯ
        float yoff = dir * speed * Time.deltaTime;
        float ypos = transform.position.y + yoff;
        if (ypos < ymin || ypos > ymax)
        {
            yoff *= -1;
            dir *= -1;
        }
        transform.Translate(0.0f, yoff, 0.0f);
    }
}