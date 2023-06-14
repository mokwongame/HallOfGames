using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RolBackGround : MonoBehaviour
{
    public Vector3 Inposition = new Vector3(11.28f, 0, 0); 
    public float RolSpeed = -0.02f;
    bool oncamera = false;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        gameObject.transform.position = gameObject.transform.position + new Vector3(RolSpeed, 0, 0);
        
        if (gameObject.transform.localPosition.x < -24.26)
        {
            gameObject.transform.localPosition = Inposition;
        }
    }
}
