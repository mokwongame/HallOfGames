// 파일 이름: MoveStone.cs
using UnityEngine;

public class MoveStone : MonoBehaviour
{
    GameManager gameManager;

    void Start()
    {
        gameManager = FindAnyObjectByType<GameManager>();
    }

    void Update()
    {
        float xoff = -gameManager.speedGoblin * Time.deltaTime;
        transform.Translate(xoff, 0.0f, 0.0f);
    }
}
