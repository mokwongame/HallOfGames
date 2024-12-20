using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MakeMissileAct : MonoBehaviour
{
    public GameObject missile;
    public GameObject smoke;
    float force = 3000.0f;
    GameManager gameManager;

    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            GameObject newMissile = Instantiate(missile, transform.position, transform.rotation);
            Rigidbody2D rb2 = newMissile.GetComponent<Rigidbody2D>();
            rb2.AddForce(Vector2.right * force);
            Instantiate(smoke, transform.position, transform.rotation);
            gameManager.playMissile();
        }
    }
}