using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static UnityEngine.RuleTile.TilingRuleOutput;

public class MakeMeteor : MonoBehaviour
{
    public GameObject meteor;
    public float startTime = 1.0f;
    float makeRate = 20.0f;
    public float ymin = 4.0f;
    public float ymax = 6.0f;
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("makeMeteorObj", startTime, makeRate);
    }

    // Update is called once per frame
    void Update()
    {
    }

    void makeMeteorObj()
    {
        Vector3 vec3;
        vec3.x = 14.5f;
        vec3.y = Random.Range(ymin, ymax);
        vec3.z = 30.0f;
        Instantiate(meteor, vec3, transform.rotation);
    }


}