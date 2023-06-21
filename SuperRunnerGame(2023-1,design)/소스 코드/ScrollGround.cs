using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScrollGround : MonoBehaviour
{
    public float speed = 3.0f;
    float width = 5.0f; // groundÀÇ Æø
    float xmin = -10.7f;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vec3 = Vector3.left * speed * Time.deltaTime;
        transform.Translate(vec3);
        if (transform.position.x < xmin - width / 2.0f)
        {
            Destroy(gameObject);
        }

    }
}
