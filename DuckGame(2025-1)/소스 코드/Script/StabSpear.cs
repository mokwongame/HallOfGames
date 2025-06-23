using UnityEngine;

public class StabSpear : MonoBehaviour
{
    public float speed = 10f;
    public float maxDistance = 2.5f;

    private Vector3 startPosition;

    GameManager gameManager;

    void Start()
    {
        startPosition = transform.position;
        gameManager = FindAnyObjectByType<GameManager>();
    }

    void Update()
    {
        // 앞으로 전진
        transform.Translate(Vector3.right * speed * Time.deltaTime);

        // 최대 거리 도달하면 파괴
        if (Vector3.Distance(startPosition, transform.position) >= maxDistance)
        {
            Destroy(gameObject);
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        string tag = collision.gameObject.tag;

        if (tag == "Apple")
        {
            gameManager.AddAppleScore();
            Destroy(collision.gameObject);
        }
        else if (tag == "Stone")
        {
            gameManager.AddStoneScore();
            Destroy(collision.gameObject);
        }

        Destroy(gameObject); // 창도 사라짐
    }
}

