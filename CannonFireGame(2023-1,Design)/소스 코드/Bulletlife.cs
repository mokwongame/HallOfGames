using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bulletlife : MonoBehaviour
{
    public AudioClip clipbomb;
    public GameObject Flame;
    GameManager gameManager;
    // Start is called before the first frame update
    void Start()
    {
        gameManager = FindObjectOfType<GameManager>();
    }

    // Update is called once per frame
    void Update()
    {
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Player") return;
        else if (collision.gameObject.tag == "Bullet") return;
        else if (collision.gameObject.tag == "Ball")
        {
            gameManager.inScore();
            Destroy(collision.gameObject, 0.1f);
        }
        else if (collision.gameObject.tag == "Wall")
        {
            gameManager.outScore();
        }
        else if (collision.gameObject.tag == "Floor")
        {
            gameManager.outScore();
        }
        Instantiate(Flame, transform.position, transform.rotation);
        playBomb();
        Destroy(gameObject);
    }

    void playBomb()
    {
        AudioSource audioSource = FindObjectOfType<AudioSource>();
        audioSource.PlayOneShot(clipbomb, 1f);
    }
}
