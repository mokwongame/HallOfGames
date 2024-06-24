using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveEnemyAct : MonoBehaviour
{
    float speed = 5.0f;
    float ymin = -4.5f;
    float ymax = 4.5f;
    int dir = 0;    // Enemy가 움직이는 방향
    int changeDirSize = 120;

    // Start is called before the first frame update
    void Start()
    {
        // dir == 1: 위쪽, dir == -1: 아래쪽, dir == 0: 정지
        dir = Random.Range(-1, 2); // -1, 0, 1(=2-1)
    }

    // Update is called once per frame
    void Update()
    {
        int rand = Random.Range(0, changeDirSize);
        if (rand == 0) dir *= -1; // changeDirSize당 한 번 정도 방향 전환
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