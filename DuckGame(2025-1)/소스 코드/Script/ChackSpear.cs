using UnityEngine;

public class ChackSpear : MonoBehaviour
{
    GameManager gameManager;

    public AudioClip sfxApple;
    public AudioClip sfxStone;

    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        string tag = collision.gameObject.tag;

        if (tag == "Apple")
        {
            gameManager.AddAppleScore();
            gameManager.ShowAppleBang(); // 🍎 호출 위치만 바뀜
            AudioSource.PlayClipAtPoint(sfxApple, Camera.main.transform.position);
            Destroy(collision.gameObject);
            Destroy(gameObject);
        }
        else if (tag == "Stone")
        {
            gameManager.AddStoneScore();
            gameManager.ShowStoneBang(); // 🪨 호출 위치만 바뀜
            AudioSource.PlayClipAtPoint(sfxStone, Camera.main.transform.position);
            Destroy(collision.gameObject);
            Destroy(gameObject);
        }
    }
}
