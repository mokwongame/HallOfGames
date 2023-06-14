using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Balllife : MonoBehaviour
{
    GameManager gameManager;
    float rotspeed;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindObjectOfType<GameManager>();
        rotspeed = Random.Range(-2f, 4f);
    }

    // Update is called once per frame
    void Update()
    {
        gameObject.transform.Rotate(0, 0, rotspeed);
        gameObject.transform.position = gameObject.transform.position + new Vector3(-0.1f, 0, 0);
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Wall")
        {
            gameManager.outScore();
            Destroy(gameObject);
        }
    }
}
