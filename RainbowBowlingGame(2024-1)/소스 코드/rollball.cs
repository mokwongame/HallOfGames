using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class rollball : MonoBehaviour
{
    float rollForce = 1500.0f;
    float movespeed = 10.0f;
    bool bRolling = false;

    GameManager gameManager;
    Rigidbody rb;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
        rb = gameObject.GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        float xoff = Input.GetAxis("Horizontal")*movespeed*Time.deltaTime;
        transform.Translate(xoff, 0.0f, 0.0f);
        if(!bRolling)
        {
            if(Input.GetKeyDown(KeyCode.Space))
            {
                rollBall();
                bRolling = true;
            }
        
        }
        else
        {
            if (transform.position.z > 26.0f)
            {
                gameManager.setMakeBall(true);
                Destroy(gameObject);
            }
        }
    }

    void rollBall()
    {
        rb.AddForce(Vector3.forward*rollForce*10.0f);
        bRolling = true;
    }
}
