using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class makeball : MonoBehaviour
{
    public GameObject ball;
    public float startTime = 1.0f;
    public float makeRate = 2.0f;
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("makeBallObj", startTime, makeRate);
    }

    void makeBallObj()
    {
        Vector3 pos = new Vector3(Random.Range(9, 14), Random.Range(-1, 8), 0);
        Instantiate(ball, pos, transform.rotation);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
