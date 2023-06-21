using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MakeGround : MonoBehaviour
{
    public GameObject ground, vine;
    public float startTimeGR = 1.0f;
    public float makeRateGR = 3.0f;
    public float startTimeVI = 2.0f;
    public float makeRateVI = 6.0f;
    public float yMaxGR = 3.0f;
    public float yMinGR = -3.0f;
    public float yMaxVI = -3.0f;
    public float yMinVI = -8.0f;
    void Start()
    {
        InvokeRepeating("makeGround", startTimeGR, makeRateGR);
        InvokeRepeating("makeVine", startTimeVI, makeRateVI);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    void makeGround()
    {
        Vector3 vec3;
        vec3.x = 10.0f;
        vec3.y = Random.Range(yMinGR, yMaxGR);
        vec3.z = 0.0f;
        Instantiate(ground, vec3, transform.rotation);
    }
    void makeVine()
    {
        Vector3 vec3;
        vec3.x = 10.0f;
        vec3.y = Random.Range(yMinVI, yMaxVI) ;
        vec3.z = 0.0f;
        Instantiate(vine, vec3, transform.rotation);
    }
}
