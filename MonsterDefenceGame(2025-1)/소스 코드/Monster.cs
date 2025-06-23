using UnityEngine;

public class Monster : MonoBehaviour
{
    public int hp = 1;
    public int scoreValue = 1;
    public float speed = 2f;

    private void OnEnable()
    {
        MonsterManager.Instance?.Register(this);
    }

    private void OnDisable()
    {
        MonsterManager.Instance?.Unregister(this);
    }

    void Update()
    {
        transform.Translate(Vector2.left * speed * Time.deltaTime);
    }

    public void TakeDamage(int damage)
    {
        hp -= damage;
        if (hp <= 0)
        {
            Die();
        }
    }

    void Die()
    {
        Debug.Log($"Monster {gameObject.name} died, adding score {scoreValue}");
        AudioManager.Instance?.PlayMonsterDeathSFX(GetMonsterType());
        ScoreManager.Instance?.AddScore(scoreValue);
        Destroy(gameObject);
    }


    int GetMonsterType()
    {
        if (gameObject.name.Contains("Monster1")) return 1;
        if (gameObject.name.Contains("Monster2")) return 2;
        if (gameObject.name.Contains("Monster3")) return 3;
        return 1;
    }
}
