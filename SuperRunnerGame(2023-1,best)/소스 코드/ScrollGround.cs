using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class ScrollGround : MonoBehaviour
{
    public float speed = 6.0f;
    float width = 20.0f;
    float xmin = -10.0f;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vec3;
        vec3 = Vector3.left * speed * Time.deltaTime;
        transform.Translate(vec3);
        
        if (transform.position.x < xmin - width / 2.0f)
        {
           Destroy(gameObject, 5.0f);
        }
    }
}
