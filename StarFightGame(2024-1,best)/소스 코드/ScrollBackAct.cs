using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScrollBackAct : MonoBehaviour
{
    float speed = 3.0f;
    float backWidth = 19.2f * 1.1f;
    float camWidth = 10.5f * 2.0f;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        Vector3 offset = Vector3.left * speed * Time.deltaTime; // 거리 = 속도*시간
        transform.Translate(offset);
        if (transform.position.x <= -camWidth / 2.0f - backWidth / 2.0f) // 이미지가 카메라를 벗어나면
        {
            transform.Translate(backWidth * 2.0f, 0.0f, 0.0f);
        }
    }
}