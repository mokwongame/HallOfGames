using UnityEngine;
using UnityEngine.UI;

public class EndSceneUI : MonoBehaviour
{
    public Image resultImage;
    public Sprite winSprite;
    public Sprite deathSprite;

    void Start()
    {
        if (GameResult.endReason == EndReason.ScoreReached)
        {
            resultImage.sprite = winSprite;
        }
        else if (GameResult.endReason == EndReason.KilledByStone)
        {
            resultImage.sprite = deathSprite;
        }
        else
        {
            Debug.LogWarning("종료 사유가 설정되지 않았습니다.");
        }
    }
}
