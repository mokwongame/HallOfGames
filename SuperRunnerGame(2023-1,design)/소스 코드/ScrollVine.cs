using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScrollVine : MonoBehaviour
{
    public float speed = 3.0f;
    float width = 2.0f; // 오브젝트의 폭
    float xmin = -10.7f;
    // Start is called before the first frame update
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
