using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScrollBack : MonoBehaviour
{
    public float speed = 1.0f;
    float width = 21.4f; // ´ÜÀ§´Â unit
    float xmin = -10.7f;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vec3 = Vector3.left* speed * Time.deltaTime;
        transform.Translate(vec3);
        if (transform.position.x < xmin - width/2.0f)
        {
            transform.Translate(width * 2.0f, 0.0f, 0.0f);
        }
    }
}
