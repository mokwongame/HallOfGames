using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaunchMissileAct : MonoBehaviour
{
    public GameObject explosion;
    GameManager gameManager;

    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>(); // FindAnyObjectByType 대신 FindObjectOfType를 사용합니다.
    }

    // Update is called once per frame
    void Update()
    {

    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        string objTag = collision.gameObject.tag;
        if (objTag == "Enemy")
        {
            Instantiate(explosion, collision.transform.position, collision.transform.rotation);
            gameManager.playExplosion();
            gameManager.incScore();
            print("score = " + gameManager.getScore());
            Destroy(gameObject, 0.1f);
            Destroy(collision.gameObject, 0.3f);
        }
        
    }
}
