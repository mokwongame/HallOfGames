using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveBilon : MonoBehaviour
{
    GameManager gameManager;
    public float speed = 1.0f;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindObjectOfType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        float xoff = -0.1f * speed * Time.deltaTime; // ��� ��ġ
        transform.Translate(xoff, 0.0f, 0.0f);
        Vector3 position = transform.position; // ���� ��ġ
        float xpos = position.x; // xpos�� ����
        xpos = Mathf.Clamp(xpos, 26.5f, 33.0f);
        //if (xpos <= -8.0f) xpos = -8.0f;
        //else if (xpos >= 8.0f) xpos = 8.0f;
        position.x = xpos;
        transform.position = position;
        //Debug.Log(xoff);
    }
}