using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class killpin : MonoBehaviour
{
    GameManager gameManager;
    bool bHit = false;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
        Destroy(gameObject, 10.0f );
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    
    private void OnCollisionEnter(Collision collision)
    {
        string objName = collision.gameObject.tag;
        print(objName);
        if (!bHit && objName == "ball") 
        {
            Color ballColor = collision.gameObject.GetComponent<Renderer>().material.color;
            Color pinColor = gameObject.GetComponent<Renderer>().material.color; ;
            bHit = true; ; 
            if (ballColor == pinColor) 
            {
                print("best hit"); 
                gameManager.incBestScore(); 
            }
            else 
            {
                print("hit"); 
                gameManager.inScore();
            }

            print("score = " + gameManager.getScore()); 
            Destroy(gameObject, 0.5f); 
        }
    }
}

