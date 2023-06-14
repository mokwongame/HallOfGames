using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MakeBall : MonoBehaviour
{
    public GameObject ball;
    public float startTime = 1.0f;
    float makeRate = 3.0f;
    public float ymin = -3.0f;
    public float ymax = 3.0f;
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("makeBallObj", startTime, makeRate);
    }

    // Update is called once per frame
    void Update()
    {

    }

    void makeBallObj()
    {
        Vector3 vec3;
        vec3.x = 11.0f;
        vec3.y = Random.Range(ymin, ymax);
        vec3.z = 0.0f;
        Instantiate(ball, vec3, transform.rotation);
    }


}