using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MakeRockAct : MonoBehaviour
{
    public GameObject rock;
    GameManager gameManager;
    float xmin = 0.0f;
    float xmax = 9.0f;
    float ymin = -4.0f;
    float ymax = 4.0f;

    // Start는 처음 프레임 업데이트 전에 호출됩니다
    void Start()
    {
        InvokeRepeating("makeRock", 1.0f, 7.0f);
        gameManager = FindAnyObjectByType<GameManager>();
    }

    // Update는 매 프레임 호출됩니다
    void Update()
    {

    }

    void makeRock()
    {
        Vector3 pos = new Vector3();
        pos.x = xmax; // 오른쪽 끝에서 생성
        pos.y = Random.Range(ymin, ymax);
        pos.z = 0.0f;

        GameObject newRock = Instantiate(rock, pos, transform.rotation);
    }
}
