using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaunchSeedAct : MonoBehaviour
{
    GameManager gameManager;

    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {

    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.CompareTag("Pot"))
        {
            gameManager.incScore();
            print("score = " + gameManager.getScore());
            Destroy(gameObject, 0.1f);
            Destroy(collision.gameObject, 0.3f);
        }
    }
}
