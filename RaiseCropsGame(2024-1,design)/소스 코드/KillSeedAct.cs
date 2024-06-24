using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KillSeedAct : MonoBehaviour
{
    float xmax = 15.0f;
    GameManager gameManager;

    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.x >= xmax)
        {
            gameManager.decScore();
            print("score = " + gameManager.getScore());
            Destroy(gameObject);
        }
    }
}

