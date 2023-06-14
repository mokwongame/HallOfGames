using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjLife : MonoBehaviour
{
    public float lifeSec = 10.0f;
    // Start is called before the first frame update
    void Start()
    {
        Destroy(gameObject, lifeSec);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
