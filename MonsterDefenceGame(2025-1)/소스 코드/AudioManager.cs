// AudioManager.cs
using UnityEngine;
using UnityEngine.SceneManagement;

public class AudioManager : MonoBehaviour
{
    public static AudioManager Instance;

    [Header("Audio Sources")]
    public AudioSource bgmSource;
    public AudioSource sfxSource;

    [Header("BGM Clips")]
    public AudioClip bgmStage1;
    public AudioClip bgmStage2;
    public AudioClip bgmStage3;
    public AudioClip bgmClearScene;
    public AudioClip bgmEndScene;

    [Header("Skill SFX Clips")]
    public AudioClip skillZClip;
    public AudioClip skillXClip;
    public AudioClip skillCClip;

    [Header("Monster Death SFX Clips")]
    public AudioClip monster1DeathClip;
    public AudioClip monster2DeathClip;
    public AudioClip monster3DeathClip;

    [Header("Floor Move SFX")]
    public AudioClip floorMoveClip;

    [Header("UI SFX Clips")]
    public AudioClip buttonClickClip;


    [Range(0f, 1f)]
    public float bgmVolume = 0.5f;
    [Range(0f, 1f)]
    public float sfxVolume = 1f;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject);
            SceneManager.sceneLoaded += OnSceneLoaded;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void OnSceneLoaded(Scene scene, LoadSceneMode mode)
    {
        PlayBGMForScene(scene.name);
    }

    public void PlayBGMForScene(string sceneName)
    {
        AudioClip clip = null;
        switch (sceneName)
        {
            case "Stage1": clip = bgmStage1; break;
            case "Stage2": clip = bgmStage2; break;
            case "Stage3": clip = bgmStage3; break;
            case "ClearScene": clip = bgmClearScene; break;
            case "EndScene": clip = bgmEndScene; break;
        }
        if (clip != null)
        {
            bgmSource.Stop();
            bgmSource.clip = clip;
            bgmSource.volume = bgmVolume;
            bgmSource.Play();
        }
    }

    public void PlaySkillSFX(string skill)
    {
        AudioClip clip = null;
        switch (skill)
        {
            case "Z": clip = skillZClip; break;
            case "X": clip = skillXClip; break;
            case "C": clip = skillCClip; break;
        }
        PlaySFX(clip);
    }

    public void PlayMonsterDeathSFX(int monsterType)
    {
        AudioClip clip = null;
        switch (monsterType)
        {
            case 1: clip = monster1DeathClip; break;
            case 2: clip = monster2DeathClip; break;
            case 3: clip = monster3DeathClip; break;
        }
        PlaySFX(clip);
    }

    public void PlaySFX(AudioClip clip)
    {
        if (clip != null)
        {
            sfxSource.PlayOneShot(clip, sfxVolume);
        }
    }

    public void SetBGMVolume(float volume)
    {
        bgmVolume = volume;
        bgmSource.volume = bgmVolume;
    }

    public void SetSFXVolume(float volume)
    {
        sfxVolume = volume;
    }

    public void PlayFloorMoveSFX()
    {
        PlaySFX(floorMoveClip);
    }

    public void PlayButtonClickSFX()
    {
        PlaySFX(buttonClickClip);
    }
}
