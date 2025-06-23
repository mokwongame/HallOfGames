using UnityEngine;

public class Magic1 : MonoBehaviour
{
    public float speed = 10f;

    void Update()
    {
        // ���������� ���ư���
        transform.Translate(Vector2.right * speed * Time.deltaTime);
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Monster"))
        {
            Monster monster = other.GetComponent<Monster>();
            if (monster != null)
            {
                monster.TakeDamage(1);  // ������ 1
            }

            Destroy(gameObject); // ���� ������Ʈ ����
        }
    }
}
